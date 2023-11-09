import math
from typing import List
from pclick import HandshakeMessage, SensorMessage, ValueType, MessageFactory as ProtoMessageFactory
from agxclick import ClickRobot, BrickUtils
from agxclick.click_robot import ClickObject


def _identity(x):
    return x


def _set_converted_signal_value(targets: List, source: List[float]):
    """
    Set signal values
    Converts Angle type values to degrees
    target is a list of Brick.Signal
    """
    for i, target in enumerate(targets):
        control_type = MessageFactory.to_click_control_type(target.__class__)
        if control_type == ValueType.Angle:
            conversion_function = math.degrees
        else:
            conversion_function = _identity

        target.SetData(conversion_function(source[i]))


def update_robots_from_message(robots: List[ClickRobot], controlmessage):
    """
    Updates a list of robots from a controlmessage
    """
    for robot in filter(lambda object: object.is_robot(), robots):
        # NOTE: We could output warning and return, but we prefer to fail to prevent user missing the problem
        assert robot.name in controlmessage.objects, f"Robot {robot.name} not found in controlmessage - client probably sent wrong name"
        control = controlmessage.objects[robot.name]
        if len(control.values):
            validate_message(controlmessage, robot, control.values)
            _set_converted_signal_value(robot.input_signals, control.values)
        else:
            # For backward compatibility and possibly performance we still support these:
            control_type = MessageFactory.to_click_control_type(robot.controlType())
            if control_type == ValueType.Angle:
                validate_message(controlmessage, robot, control.angles)
                _set_converted_signal_value(robot.input_signals, control.angles)
            elif control_type == ValueType.AngleVelocity:
                validate_message(controlmessage, robot, control.angleVelocities)
                _set_converted_signal_value(robot.input_signals, control.angleVelocities)
            elif control_type == ValueType.Torque:
                validate_message(controlmessage, robot, control.torques)
                _set_converted_signal_value(robot.input_signals, control.torques)
            else:
                raise Exception(f"Updating robot from controltype {robot.controlType()} has not been implemented")
        for key, enabled in control.controlEvents.items():
            if type(robot.control_events()[key].GetData()) == bool:
                robot.control_events()[key].SetData(enabled)
            else:
                robot.control_events()[key].SetData(100.0 if enabled else 0.0)


def validate_message(controlmessage, robot, values):
    assert len(robot.input_signals) == len(values), f"Mismatching number of values for {robot.name} in controlmessage, got {len(values)} should be {len(robot.input_signals)} - {controlmessage}"


class MessageFactory:

    @classmethod
    def to_click_control_types(cls, types: List) -> ValueType:
        return list(map(lambda x: cls.to_click_control_type(x.__class__), types))

    @classmethod
    def to_click_control_type(cls, type) -> ValueType:
        """
        Convert a Brick type to the corresponding click ValueType
        type is a Brick.Signal.*Input
        """
        if not hasattr(cls, "typemap"):
            Brick = BrickUtils.import_Brick()
            cls.typemap = {
                Brick.Signal.LockPositionInput: ValueType.Angle,
                Brick.Signal.VelocityInput: ValueType.AngleVelocity,
                Brick.Signal.ForceInput: ValueType.Torque,
                Brick.Signal.MotorVelocityInput: ValueType.AngleVelocity,
                Brick.Signal.FixedVelocityEngineInput: ValueType.AngleVelocity,
                Brick.Signal.EngineTorqueInput: ValueType.Torque,
                Brick.Signal.MotorForceInput: ValueType.Torque,
                # NOTE: AdhesiveForceInput is mapped to Activated to support suction cup, should be more generic, ie might not always want to map Adhesive to bool
                Brick.Signal.AdhesiveForceInput: ValueType.Activated,
                Brick.Signal.ForceVectorOutput: ValueType.Force,
                Brick.Signal.TorqueVectorOutput: ValueType.DirectionalTorque,
                Brick.Signal.RotatingBodyAngleOutput: ValueType.Angle,
                Brick.Signal.RotatingBodyVelocityOutput: ValueType.AngleVelocity,
                Brick.Signal.FixedVelocityEngineTorqueOutput: ValueType.Torque,
                Brick.Signal.ComponentBoolInput: ValueType.Activated,
                Brick.Signal.ComponentBoolOutput: ValueType.Activated,
            }
        return cls.typemap[type]

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
        Convert data of Brick.Signal.ConnectorVectorOutput Brick.Math.Vec3 to list of 3 floats
        """
        Brick = BrickUtils.import_Brick()
        data = signal.GetData()
        if data.__class__ is Brick.Math.Vec3:
            return [data.X, data.Y, data.Z]
        return data

    @classmethod
    def handshake_message_from_objects(cls, objects: List[ClickObject], timeStep) -> HandshakeMessage:
        """
        Creates a HandshakeMessage from a list of robots
        """
        handshake = ProtoMessageFactory.create_handshake()
        handshake.controlType = cls.check_uniform_controltype(objects)
        handshake.simSettings.timeStep = timeStep
        for robot in objects:
            object = handshake.objects[robot.name]
            if robot.is_robot():
                object.controlsInOrder.extend(robot.joint_protocolrefs())
                object.controlTypesInOrder.extend(cls.to_click_control_types(robot.input_signals))
                object.jointSensorsInOrder.extend(robot.joint_protocolrefs())
                jointsensors = list()
                if len(robot.angle_sensors) > 0:
                    jointsensors.append(ValueType.Angle)
                if len(robot.velocity_sensors) > 0:
                    jointsensors.append(ValueType.AngleVelocity)
                if len(robot.torque_sensors) > 0:
                    jointsensors.append(ValueType.Torque)
                object.jointSensors.extend(jointsensors)
                for name, event in robot.control_events().items():
                    object.controlEvents[name] = cls.to_click_control_type(event.__class__)
                for name, sensor in robot.sensors.items():
                    object.sensors[name].types.extend(cls.to_click_control_types(sensor))
            object.objectSensors.append(ValueType.Position)
            object.objectSensors.append(ValueType.RPY)
        return handshake

    @classmethod
    def check_uniform_controltype(cls, objects):
        """
        For backward compatibility we use the specific controlType if all signals are the same type
        """
        robots = list(filter(lambda robot: robot.is_robot(), objects))
        control_types = {type for robot in robots for type in robot.control_types()}
        if len(control_types) == 1:
            return cls.to_click_control_type(robots[0].controlType())
        return ValueType.Multiple

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
                cls.add_named_sensors(robot, sensors)
            objectSensor = sensors.objectSensors.add()
            objectSensor.position.arr.extend([*robot.position()])
            objectSensor = sensors.objectSensors.add()
            objectSensor.rpy.arr.extend([*robot.rpy()])

        return sensor_m

    @classmethod
    def add_named_sensors(cls, robot: ClickRobot, sensors):
        for name, signals in robot.sensors.items():
            for signal in signals:
                sensor = sensors.sensors[name].sensor.add()
                value_type = cls.to_click_control_type(signal.__class__)
                if value_type == ValueType.Angle:
                    sensor.angle = signal.GetData()
                elif value_type == ValueType.Torque:
                    sensor.torque = signal.GetData()
                elif value_type == ValueType.AngleVelocity:
                    sensor.angleVelocity = signal.GetData()
                else:
                    arr = cls.array_to_populate(signal, value_type, sensor, robot.name)
                    arr.extend(cls._signal_to_floats(signal))

    @classmethod
    def array_to_populate(cls, signal, value_type, sensor, robotname):
        if value_type == ValueType.Force:
            return sensor.force.arr
        elif value_type == ValueType.DirectionalTorque:
            return sensor.directionalTorque.arr
        elif value_type == ValueType.Angle:
            return sensor.angle
        else:
            # TODO: Add tests for rest of valuetypes, missing 8 so far.
            raise Exception(f"Unrecognized (=Not Implemented) signal in {robotname}: {signal._ModelType}")
