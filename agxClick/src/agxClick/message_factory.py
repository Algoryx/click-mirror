from typing import List, Any
from agxClick import ClickRobot
from pClick import HandshakeMessage, SensorMessage, ValueType, MessageFactory as ProtoMessageFactory
import math


def _identity(x):
    return x


def _signals2float(targets: List, source: List[float], conversion_function=_identity):
    """
    target is a list of Brick.Signal
    """
    for i, target in enumerate(targets):
        target.SetData(conversion_function(source[i]))


def update_robots_from_message(robots: List[ClickRobot], controlmessage):
    """
    Updates a list of robots from a controlmessage
    """
    for robot in filter(lambda object: object.is_robot(), robots):
        control = controlmessage.objects[robot.name]
        control_type = MessageFactory.to_click_control_type(robot.controlType())
        if control_type == ValueType.Angle:
            validate_message(controlmessage, robot, control.angles)
            _signals2float(robot.input_signals, control.angles, math.degrees)
        elif control_type == ValueType.AngleVelocity:
            validate_message(controlmessage, robot, control.angleVelocities)
            _signals2float(robot.input_signals, control.angleVelocities)
        elif control_type == ValueType.Torque:
            validate_message(controlmessage, robot, control.torques)
            _signals2float(robot.input_signals, control.torques)
        else:
            raise Exception(f"Updating robot from controltype {robot.controlType()} has not been implemented")
        for key, enabled in control.controlEvents.items():
            robot.control_events()[key].SetData(100.0 if enabled else 0.0)


def validate_message(controlmessage, robot, values):
    assert len(robot.input_signals) == len(values), f"Missing values for {robot.name} in controlmessage, got {len(values)}/{len(robot.input_signals)} - {controlmessage}"


class MessageFactory:

    @classmethod
    def to_click_control_type(cls, type) -> ValueType:
        """
        Convert a Brick type to the corresponding click ValueType
        type is a Brick.Signal.*Input
        """
        import Brick.Signal
        typemap = {
            Brick.Signal.LockPositionInput: ValueType.Angle,
            Brick.Signal.MotorVelocityInput: ValueType.AngleVelocity,
            Brick.Signal.MotorForceInput: ValueType.Torque,
            # NOTE: This is implemented like this to support suction cup, should be more generic, ie might not always want to map Adhesive to bool
            Brick.Signal.AdhesiveForceInput: ValueType.Activated,
            Brick.Signal.ForceVectorOutput: ValueType.Force,
            Brick.Signal.TorqueVectorOutput: ValueType.DirectionalTorque
        }
        return typemap[type]

    @classmethod
    def to_click_control_types(cls, signals: List) -> List:
        return list(map(lambda signal: cls.to_click_control_type(signal.__class__), signals))

    @classmethod
    def to_brick_control_type(cls, type: ValueType):
        """
        Convert a click ValueType to the corresponding Brick type
        return a Brick.Signal.*Input type
        """
        import Brick.Signal
        typemap = {
            ValueType.Angle: Brick.Signal.LockPositionInput,
            ValueType.Torque: Brick.Signal.MotorForceInput,
            ValueType.AngleVelocity: Brick.Signal.MotorVelocityInput
        }
        return typemap[type]

    @classmethod
    def _degs_to_radians(cls, signals) -> List[float]:
        """
        Convert a list of Brick signals in degrees to radians
        return a Brick.Signal.*Input type
        """
        return list(map(lambda signal: math.radians(signal.GetData()), signals))

    @classmethod
    def _signals_to_floats(cls, signals) -> List[float]:
        """
        Convert a list of Brick signals to List of floats
        return a Brick.Signal.*Input type
        """
        return list(map(lambda signal: cls._signal_to_floats(signal), signals))

    @classmethod
    def _signal_to_floats(cls, signal):
        """
        Convert data of Brick.Signal.ConnectorVectorOutput Brick.Math.Vec3 to
        """
        import Brick.Math
        data = signal.GetData()
        if data.__class__ is Brick.Math.Vec3:
            return [data.X, data.Y, data.Z]
        return data

    @classmethod
    def handshake_message_from_objects(cls, robots: List[ClickRobot], timeStep) -> HandshakeMessage:
        """
        Creates a HandshakeMessage from a list of robots
        """
        handshake = ProtoMessageFactory.create_handshake()
        handshake.controlType = cls.to_click_control_type(robots[0].controlType())
        handshake.simSettings.timeStep = timeStep
        for robot in robots:
            object = handshake.objects[robot.name]
            if robot.is_robot():
                object.controlsInOrder.extend(robot.joint_protocolrefs())
                object.jointSensorsInOrder.extend(robot.joint_protocolrefs())
                jointsensors = list()
                if len(robot.angle_sensors) > 0:
                    jointsensors.append(ValueType.Angle)
                if len(robot.velocity_sensors) > 0:
                    jointsensors.append(ValueType.AngleVelocity)
                if len(robot.torque_sensors) > 0:
                    jointsensors.append(ValueType.Force)
                object.jointSensors.extend(jointsensors)
                for name, event in robot.control_events().items():
                    object.controlEvents[name] = cls.to_click_control_type(event.__class__)
                for name, sensor in robot.sensors.items():
                    object.sensors[name].types.extend(cls.to_click_control_types(sensor))
            object.objectSensors.append(ValueType.Position)
            object.objectSensors.append(ValueType.RPY)
        return handshake

    @classmethod
    def sensor_message_from_objects(cls, robots: List[ClickRobot], simulated_time) -> SensorMessage:
        """
        Create a SensorMessage from a list of robots
        """

        sensor_m = ProtoMessageFactory.create_sensormessage()
        sensor_m.simVars.simulatedTime = simulated_time

        for robot in robots:
            sensors = sensor_m.objects[robot.name]
            if robot.is_robot():
                if len(robot.angle_sensors) > 0:
                    sensors.angleSensors.extend(cls._degs_to_radians(robot.angle_sensors))
                if len(robot.velocity_sensors) > 0:
                    sensors.angleVelocitySensors.extend(cls._signals_to_floats(robot.velocity_sensors))
                if len(robot.torque_sensors) > 0:
                    sensors.torqueSensors.extend(cls._signals_to_floats(robot.torque_sensors))
                for name, signals in robot.sensors.items():
                    sensors.sensors[name].sensor.extend(cls._signals_to_floats(signals))
            objectSensor = sensors.objectSensors.add()
            objectSensor.position.arr.extend([*robot.position()])
            objectSensor = sensors.objectSensors.add()
            objectSensor.rpy.arr.extend([*robot.rpy()])

        return sensor_m
