import queue
import agxSDK
from typing import List, Callable
from agxClick import MessageFactory, ApplicationStepListener, update_robots_from_message, find_robots_in_scene, has_click_configuration, ClickObject, noop
from pClick import Server, HandshakeInitMessageType, ControlMessageType, ResetMessageType, SensorRequestMessageType, ErrorMessageType
from pClick import ControlMessage, MessageFactory as ProtoMessageFactory
import logging
from queue import SimpleQueue
from enum import Enum, auto

from agxClick import get_click_configuration


class States(Enum):
    INVALID = auto()

    RECV_HANDSHAKE = auto()
    RECV = auto()

    READ_CONTROLS = auto()
    SEND_SENSORS = auto()
    SEND_RESET = auto()

    def valid(self):
        return self is not self.INVALID

    def can_receive(self):
        return self is self.RECV or self is self.RECV_HANDSHAKE or self is self.SEND_RESET

    def can_send(self):
        return self is self.SEND_SENSORS or self is self.READ_CONTROLS


# class ClickFrameListener(agxOSG.ExampleApplicationListener):
class ClickFrameListener(ApplicationStepListener):
    def __init__(self, scene, app,
                 on_stop: Callable[[], None] = noop, 
                 on_exception: Callable[[Exception], None] = noop,
                 on_reset: Callable[[], None] = noop):
        """
        scene is the top Brick.Component of the scene
        app is the running ExampleApplication
        sim is the agx simulation
        on_stop is an optional function to call on ErrorMessage, ie when stopping application
        on_exception is an optional function to call when exception occurs
        on_reset is an optional function to call when a ResetMessage is received
        """
        super().__init__()
        self._logger = logging.getLogger(__file__)
        self._app = app
        self._on_stop = on_stop
        self._on_exception = on_exception
        self._on_reset = on_reset
        if scene is None:
            self._logger.warning("Scene is None, update_scene() must be called before first application step!")
            self._click_objects = None
        else:
            self.update_scene(scene)

        self._server = Server(f"tcp://*:5555")
        self.control_queue: SimpleQueue[ControlMessage] = SimpleQueue()
        self.click_event_listener = ClickEventListener(self._server, self._click_objects, self.control_queue, self._on_exception)
        self._app.getSimulation().addEventListener(self.click_event_listener)
        self.num_controls_received = 0

    @property
    def _state(self):
        return self.click_event_listener.state

    @state.setter
    def _state(self, new_state: States):
        self.click_event_listener.state = new_state

    def update_scene(self, scene):
        if (has_click_configuration(scene)):
            self._click_objects: List[ClickObject] = get_click_configuration(scene)
        else:
            self._click_objects: List[ClickObject] = find_robots_in_scene(scene)
        self.click_event_listener._click_objects = self._click_objects
        self._retrieve_initial_values()

    def _retrieve_initial_values(self):
        from Brick.AGXBrick import BrickSimulation
        BrickSimulation.Default.SyncOutputParameters()

    def can_step_simulation(self):
        return self._state is States.READ_CONTROLS

    def handshake_completed(self):
        return self._state is not States.RECV_HANDSHAKE

    def preFrame(self, time: float):
        try:
            if not self._state.can_receive():
                return
            request = self._server.recv()
            if request is None:
                return
            if request.messageType == HandshakeInitMessageType:
                self._logger.info("Received handshake init message")
                message = MessageFactory.handshake_message_from_objects(self._click_objects, self._app.getSimulation().getTimeStep())
                self._server.send(message)
                if self._state == States.SEND_RESET:
                    self._state = States.RECV
            elif self._state == States.SEND_RESET:
                self._send_resetmessage()
            elif request.messageType == SensorRequestMessageType:
                message = MessageFactory.sensor_message_from_objects(self._click_objects, self._app.getSimulation().getClock().getTime())
                self._logger.info(f"Received Sensor Request message, sending {message}")
                self._server.send(message)
            elif request.messageType == ControlMessageType:
                self.num_controls_received += 1
                self.control_queue.put(request)
                if self._state == States.RECV_HANDSHAKE:
                    self._logger.info(f"Handshake completed")
                self._state = States.READ_CONTROLS
            elif request.messageType == ErrorMessageType:
                self._logger.warning("Received Error message")
                self._on_stop()
            elif request.messageType == ResetMessageType:
                self._logger.info("Received Reset message")
                self._on_reset()
                self._server.send(ProtoMessageFactory.create_resetmessage())
            else:
                self._logger.warning(f"Ignoring message: \n{request}\nSending errormessage as reply")
                self._server.send(ProtoMessageFactory.create_errormessage())
        except Exception as ex:
            self._logger.info(f"Exception encountered - Stopping click messaging")
            self._state = States.INVALID
            self._on_exception(ex)
            raise ex

    def _send_resetmessage(self):
        self._logger.info(f"Sending reset message")
        self._server.send(ProtoMessageFactory.create_resetmessage())
        self._state = States.RECV

    def send_reset(self):
        if self._state.can_send():
            self._send_resetmessage()
        else:
            self._logger.info(f"Sending Reset as next Response (unless Handshake encountered)")
            self._state = States.SEND_RESET

        try:
            self.control_queue.get(block=False)
        except queue.Empty as ex:
            self._logger.info(f"ControlMessage queue emptied because of Reset")
            assert self.control_queue.empty()

    def stop(self):
        self._server.stop()


class ClickEventListener(agxSDK.StepEventListener):
    def __init__(self, server: Server, click_objects: List[ClickObject], control_queue: SimpleQueue, on_exception: Callable[[Exception], None] = noop):
        """
        """
        super().__init__()
        self._server = server
        self._click_objects = click_objects
        self._control_queue: SimpleQueue = control_queue
        self._on_exception = on_exception
        self._logger = logging.getLogger(__file__)
        # The state is shared by both listeners
        self.state = States.RECV_HANDSHAKE

    def pre(self, time: float):
        if self.state is not States.READ_CONTROLS:
            return
        try:
            self._logger.debug(f"Updating click objects")
            request = self._control_queue.get(block=False)
            assert self._control_queue.empty()

            update_robots_from_message(self._click_objects, request)
            self.state = States.SEND_SENSORS
        except queue.Empty as ex:
            self._logger.fatal(f"No ControlMessage in queue")
            self.state = States.INVALID
            self._on_exception(ex)
            raise ex
        except Exception as ex:
            self._logger.fatal(f"Exception {str(ex)} encountered - Stopping click messaging")
            self.state = States.INVALID
            self._on_exception(ex)
            raise ex

    def post(self, time: float):
        if self.state is not States.SEND_SENSORS:
            return
        try:
            self._logger.debug(f"Sending sensor message")
            response = MessageFactory.sensor_message_from_objects(self._click_objects, time)
            self._server.send(response)
            self.state = States.RECV
        except Exception as ex:
            self._logger.info(f"Exception encountered - Stopping click messaging")
            self.state = States.INVALID
            self._on_exception(ex)
            raise ex
