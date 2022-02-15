import queue
import agxSDK
import agxOSG
from typing import List, Callable
from agxClick import MessageFactory, update_robots_from_message, find_robots_in_scene, has_click_configuration, ClickObject
from pClick import Server, HandshakeInitMessageType, ControlMessageType, ResetMessageType, SensorRequestMessageType, ErrorMessageType
from pClick import ControlMessage, MessageFactory as ProtoMessageFactory
import logging
from queue import SimpleQueue

from agxClick import get_click_configuration


def noop(*args, **kwargs: None):
    pass


# Remove when https://git.algoryx.se/algoryx/agx/-/merge_requests/2401 is released
_REGISTER_FRAME_LISTENER = False
if _REGISTER_FRAME_LISTENER:
    _parent = agxOSG.ExampleApplicationListener
else:
    _parent = object


# class ClickFrameListener(agxOSG.ExampleApplicationListener):
class ClickFrameListener(_parent):
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
        self.handshake_completed = False
        self.num_controls_received = 0
        self.valid = True

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

    def step_simulation(self):
        return self.click_event_listener.pending_send

    def preFrame(self, time: float):
        if not self.valid:
            return
        try:
            # Skip until message can be received
            if self.click_event_listener.pending_send:
                return
            request = self._server.recv()
            if request is None:
                return

            if request.messageType == HandshakeInitMessageType:
                self._logger.info("Received handshake init message")
                message = MessageFactory.handshake_message_from_objects(self._click_objects, self._app.getSimulation().getTimeStep())
                self._server.send(message)
            elif request.messageType == SensorRequestMessageType:
                message = MessageFactory.sensor_message_from_objects(self._click_objects, self._app.getSimulation().getClock().getTime())
                self._logger.info(f"Received Sensor Request message, sending {message}")
                self._server.send(message)
            elif request.messageType == ControlMessageType:
                self.num_controls_received += 1
                self.control_queue.put(request)
                self.click_event_listener.pending_send = True
                if not self.handshake_completed:
                    self.handshake_completed = True
                    self._logger.info(f"Handshake completed")
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
            self.valid = False
            self._on_exception(ex)
            raise ex

    def send_reset(self):
        self.click_event_listener.send_reset = True

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
        self.pending_send = False
        self.send_reset = False
        self.valid = True

    def pre(self, time: float):
        if not self.valid or not self.pending_send:
            return
        try:
            self._logger.debug(f"Updating click objects")
            request = self._control_queue.get(block=False)
            assert self._control_queue.empty()
            if self.send_reset:
                self._logger.info(f"Skipping update_robots... from ControlMessage - haven't sent Reset yet")
                return

            update_robots_from_message(self._click_objects, request)
        except queue.Empty as ex:
            self._logger.fatal(f"No ControlMessage in queue")
            self.valid = False
            self._on_exception(ex)
            raise ex
        except Exception as ex:
            self._logger.fatal(f"Exception {str(ex)} encountered - Stopping click messaging")
            self.valid = False
            self._on_exception(ex)
            raise ex

    def post(self, time: float):
        if not self.valid or not self.pending_send:
            return
        try:
            if self.send_reset:
                self._logger.info(f"Sending reset message")
                response = ProtoMessageFactory.create_resetmessage()
                self.send_reset = False
            else:
                self._logger.debug(f"Sending sensor message")
                response = MessageFactory.sensor_message_from_objects(self._click_objects, time)
            self._server.send(response)
            self.pending_send = False
        except Exception as ex:
            self._logger.info(f"Exception encountered - Stopping click messaging")
            self.value = False
            self._on_exception(ex)
            raise ex
