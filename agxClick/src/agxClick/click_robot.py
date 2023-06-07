from typing import Any, List, Tuple, Dict
from agxclick import BrickUtils
import logging
import itertools
from pclick import ValueType


class ClickObject():
    def __init__(self, component):
        """
        component is a Brick.Component
        """
        self.component = component
        self.name = component.Name
        self.logger = logging.getLogger(__file__)

    def position(self) -> Tuple[float, float, float]:
        pos = self.component.Transform.Position  # Brick.Math.Vec3
        return (pos.X, pos.Y, pos.Z)

    def rpy(self) -> Tuple[float, float, float]:
        return BrickUtils.roll_pitch_yaw(self.component.Transform.Rotation)

    def is_robot(self):
        return False

    def __str__(self):
        def classname(cls):
            return f"{cls.__class__.__module__}.{cls.__class__.__name__}"

        def values(signals):
            return list(map(lambda signal: signal.GetData(), signals))

        def signalstr(signals):
            return f"{classname(signals[0])}: {values(signals)}"

        def eventstr(tuple: Tuple[str, Any]):
            key, value = tuple
            return f"{key}: {value.GetData()}"

        return f"name: {self.name} with component {self.component.__class__}"


class ClickRobot(ClickObject):
    def __init__(self, brick_robot):
        """
        brick_robot must be instance of Brick.Robotics.Robot, typically a Brick.AGXBrick.UrdfModel
        Throws exception if
          - Any unknown signal (unknown=not implemented interpretation of) is found in robot
          - Number of input signals or sensors does not match number of joints
        """
        super().__init__(brick_robot)
        Brick = BrickUtils.import_Brick()
        assert isinstance(brick_robot, Brick.Robotics.Robot)
        self.brickrobot = brick_robot
        self.input_signals: List[Brick.Signal.LockPositionInput | Brick.Signal.ForceInput | Brick.Signal.VelocityInput] = []
        self.torque_sensors: List[Brick.Signal.ForceScalarOutput] = []    # Nm
        self.angle_sensors: List[Brick.Signal.MotorAngleOutput] = []     # DEG in Brick
        self.velocity_sensors: List[Brick.Signal.MotorVelocityOutput] = []  # RAD/s in Brick
        self.num_joints = len(self.joints())
        self.control_event_dict = {}
        self.sensors: Dict[str, List[Brick.Signal.Output]] = {}
        self._populate_signals()
        self.validate()

    def is_robot(self):
        return True

    def validate(self):
        """
        Throws exception if Robot is not in a valid state
        """
        assert len(self.name) > 1, "Invalid robot name '{self.name}'"
        assert self.num_joints == len(self.input_signals), f"Number of input_signals {len(self.input_signals)} did not match number of joints {self.num_joints}"
        if (self.torque_sensors):
            assert self.num_joints == len(self.torque_sensors), f"Number of torque_sensors {len(self.torque_sensors)} did not match number of joints {self.num_joints}"
        if (self.angle_sensors):
            assert self.num_joints == len(self.angle_sensors), f"Number of angle_sensors {len(self.angle_sensors)} did not match number of joints {self.num_joints}"
        if (self.velocity_sensors):
            assert self.num_joints == len(self.velocity_sensors), f"Number of velocity_sensors {len(self.velocity_sensors)} did not match number of joints {self.num_joints}"

        assert None not in self.joint_protocolrefs(), f"Missing protocolReference in robot {self.name}, refs are: {self.joint_protocolrefs()}"

    def joints(self) -> List[Any]:
        def flatten(lists: List[List]) -> List:
            return list(itertools.chain.from_iterable(lists))
        return flatten(map(lambda arm: arm.Joints, self.brickrobot.Arms))

    def joint_protocolrefs(self) -> List[str]:
        """
        Returns protocolReference name of all joints in a sorted list
        """
        return list(map(lambda joint: joint.ProtocolReference, self.joints()))

    def control_event_names(self) -> List[str]:
        return self.control_event_dict.keys()

    def control_events(self) -> Dict:
        return self.control_event_dict

    def has_control_events(self):
        return bool(self.control_event_dict)

    def joint_sensors(self) -> List[Tuple[str, Any]]:
        """
            returns list with tuples (protocolReference, OutputSignal: Brick.Signal.*Output)
        """
        return list(map(lambda sensor: (sensor.Joint.ProtocolReference, sensor.GetOrCreateSignal()), self.brickrobot.Sensors))

    def controlType(self):
        """
        DEPRECATED: Use control_types()
        Returns how this robot expects to be controlled
        return a Brick.Signal.SignalBase, one of Brick.Signal.*Input, see https://brick:ien1ieh7Cithoohoh2AhNg0waingaigu@d2epgodm7v8ass.cloudfront.net/Introduction.html under Modules->Signal
            - Signal.LockPositionInput
            - Signal.MotorForceInput
            - Signal.MotorVelocityInput
        """
        control_types = set(self.control_types())
        assert len(control_types) > 0, f"Misconfiguration: Robot {self.name} has no input signals"
        assert len(control_types) == 1, f"Robot {self.name} has multiple controltypes - use control_types()"
        return control_types.pop()

    def control_types(self) -> List:
        """
        Returns how this robot expects to be controlled
        return a List[Brick.Signal.SignalBase], one of Brick.Signal.*Input, see https://brick:ien1ieh7Cithoohoh2AhNg0waingaigu@d2epgodm7v8ass.cloudfront.net/Introduction.html under Modules->Signal
            - Signal.LockPositionInput
            - Signal.MotorForceInput
            - Signal.MotorVelocityInput
        """
        return [s.__class__ for s in self.input_signals]

    def __str__(self):
        def classname(cls):
            return f"{cls.__class__.__module__}.{cls.__class__.__name__}"

        def values(signals):
            return list(map(lambda signal: signal.GetData(), signals))

        def types(signals):
            return list(map(lambda signal: classname(signal), signals))

        def signalstr(signals):
            if len(signals) == 0:
                return "[]"
            return f"{classname(signals[0])}: {values(signals)}"

        def eventstr(tuple: Tuple[str, Any]):
            key, value = tuple
            return f"{key}: {value.GetData()}"

        return f"""name: {self.name}
    num_joints: {self.num_joints}
    jointnames: {self.joint_protocolrefs()}
    control input type: {self.controlType() if len(set(self.control_types())) == 1 else "Multiple"}
    {len(self.input_signals)} input_types: {types(self.input_signals)}
    {len(self.input_signals)} input_values: {values(self.input_signals)}
    {len(self.torque_sensors)} torque_sensors: {signalstr(self.torque_sensors)}
    {len(self.angle_sensors)} angle_sensors: {signalstr(self.angle_sensors)}
    {len(self.velocity_sensors)} velocity_sensors: {signalstr(self.velocity_sensors)}
    control events: {list(map(eventstr, self.control_events().items()))}
"""

    def _populate_signals(self):
        for signal in self.brickrobot.GetSignals():
            self._populate_signal(signal)

    def _sensor_of_or(self, signal, default_reply):
        for sensor in self.brickrobot.Sensors:
            if signal == sensor.GetOrCreateSignal():
                return sensor
        return default_reply

    def _sensor_of(self, signal):
        for sensor in self.brickrobot.Sensors:
            if signal == sensor.GetOrCreateSignal():
                return sensor
        raise Exception("Signal {signal} was not part of any sensor in {self.name}")

    def _populate_signal(self, signal):
        Brick = BrickUtils.import_Brick()
        sensor = self._sensor_of_or(signal, object)
        if isinstance(sensor, Brick.Robotics.Sensor.JointSensor):
            if signal.__class__ is Brick.Signal.MotorAngleOutput:
                self.angle_sensors.append(signal)
            elif signal.__class__ is Brick.Signal.MotorVelocityOutput:
                self.velocity_sensors.append(signal)
            elif isinstance(signal, Brick.Signal.ForceScalarOutput):
                self.torque_sensors.append(signal)
        elif isinstance(signal, (Brick.Signal.LockPositionInput,
                                 Brick.Signal.ForceInput,
                                 Brick.Signal.VelocityInput)):
            self.input_signals.append(signal)
        elif signal.__class__ is Brick.Signal.AdhesiveForceInput:
            shortname = signal['name']
            if shortname == "":
                shortname = str(signal._ModelValuePath).rsplit('.', 1)[-1]
            self.logger.info(f"Configuring signal {signal._ModelValuePath} as controlEvent, will accept {shortname} in controlmessage for {self.name}")
            self.control_event_dict[shortname] = signal
        elif signal.__class__ is Brick.Signal.ComponentBoolInput or \
                signal.__class__ is Brick.Signal.ComponentBoolOutput:
            shortname = signal['name']
            if shortname == "":
                shortname = str(signal._ModelValuePath).rsplit('.', 1)[-1]
            self.logger.info(f"Configuring signal {signal._ModelValuePath} as controlEvent, will accept {shortname} in controlmessage for {self.name}")
            self.control_event_dict[shortname] = signal
        elif isinstance(signal, (Brick.Signal.ConnectorVectorOutput,
                                 Brick.Signal.PositionScalarOutput,
                                 Brick.Signal.VelocityScalarOutput,
                                 Brick.Signal.ForceScalarOutput)):
            id = self._sensor_of(signal)['protocolReference']
            assert id, f"Missing protocolReference in robot {self.name} sensor {self._sensor_of(signal)['name']}"
            self.logger.info(f"Configuring signal {signal._ModelValuePath} as sensor, with name {id} in {self.name}")
            if id in self.sensors:
                self.sensors[id].append(signal)
            else:
                self.sensors[id] = [signal]
        else:
            raise Exception(f"Unrecognized signal in {self.name}: {signal._ModelType}, class: {signal.__class__}")
