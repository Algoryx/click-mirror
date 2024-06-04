from typing import List
from os import chdir
import pytest
from pytest import approx
from pclick import ValueType, MessageFactory as ProtoMessageFactory, ControlMessage
from agxclick import MessageFactory, update_robots_from_message, find_robots_in_scene, get_click_configuration, ClickRobot


# Kept for backward compatibility test
def create_faked_controlmessage_for(robots: List[ClickRobot], add_control_event=True) -> ControlMessage:
    """
    add_control_event, if True an event will be added with False for first robot, True for rest
    """
    control_m = ProtoMessageFactory.create_controlmessage()
    for i, robot in enumerate(robots):
        control = control_m.objects[robot.name]
        controltype = MessageFactory.to_click_control_type(robot.controlType())
        if controltype == ValueType.Angle:
            control.angles.extend([x * 1.0 for x in range(1, robot.num_joints + 1)])
        elif controltype == ValueType.AngularVelocity:
            control.angularVelocities.extend([x * 2.0 for x in range(1, robot.num_joints + 1)])
        elif controltype == ValueType.Torque:
            control.torques.extend([x * 3.0 for x in range(1, robot.num_joints + 1)])
        else:
            raise Exception(f"Faking {robot.controlType()} has not been implemented")
        if add_control_event:
            for name in robot.control_events().keys():
                control.controlEvents[name] = i
    return control_m


def create_faked_controlmessage_persignal_for(robots: List[ClickRobot], add_control_event=True):
    """
    add_control_event, if True an event will be added with False for first robot, True for rest
    """
    control_m = ProtoMessageFactory.create_controlmessage()
    j = 1
    for i, robot in enumerate(robots):
        control = control_m.objects[robot.name]
        for _ in robot.input_signals:
            control.values.append(j * 1.3)
            j += 1
        if (add_control_event):
            for name in robot.control_events().keys():
                control.controlEvents[name] = i
    return control_m


@pytest.mark.integrationtest
class Test_handshake_message_from_objects:

    @pytest.fixture(scope="class", autouse=True)
    def exec_from_brick_config_expected_path(self, request):
        print(f"Changing exec directory from {request.config.invocation_dir} to {request.fspath.dirname}/..")
        chdir(f"{request.fspath.dirname}/..")
        yield
        chdir(request.config.invocation_dir)

    def test_joints(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        assert [x * 1.0 for x in range(1, robots[0].num_joints + 1)] == [1.0, 2.0]

    def test_that_generating_handshake_creates_correct_handshake(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert str(message) == _handshake_facit

    def test_that_generating_handshake_creates_correct_handshake_velocity_input(self, scene_velocityinput):
        robots = find_robots_in_scene(scene_velocityinput)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert message.controlType == ValueType.AngularVelocity

    def test_that_generating_handshake_creates_correct_handshake_force_input(self, scene_forceinput):
        robots = find_robots_in_scene(scene_forceinput)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert message.controlType == ValueType.Torque

    def test_that_generating_handshake_creates_multiple_input_types(self, scene_position_velocity_force_input):
        robots = find_robots_in_scene(scene_position_velocity_force_input)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert message.objects["robot1"].controlTypesInOrder[0] == ValueType.Angle
        assert message.objects["robot1"].controlTypesInOrder[1] == ValueType.AngularVelocity
        assert message.objects["robot2"].controlTypesInOrder[0] == ValueType.Angle
        assert message.objects["robot2"].controlTypesInOrder[1] == ValueType.Torque

    def test_that_handshake_with_multiple_input_types_has_no_robot_controltype(self, scene_position_velocity_force_input):
        robots = find_robots_in_scene(scene_position_velocity_force_input)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)

        assert message.controlType == ValueType.Multiple

    def test_that_generating_handshake_creates_correct_handshake_sensor_output(self, sensor_scene):
        robots = find_robots_in_scene(sensor_scene)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert ValueType.Force in message.objects["robot"].sensors["forceTorqueSensor"].types
        assert ValueType.DirectionalTorque in message.objects["robot"].sensors["forceTorqueSensor"].types

    def test_that_click_box_is_included_in_handshake(self, click_scene):
        objects = get_click_configuration(click_scene)

        message = MessageFactory.handshake_message_from_objects(objects, 0.03)

        assert ValueType.Position in message.objects["box"].objectSensors
        assert ValueType.RPY in message.objects["box"].objectSensors
        assert message.objects["box"].objectSensors == [ValueType.Position, ValueType.RPY]


@pytest.mark.integrationtest
class Test_update_robots_from_message:

    def test_that_generating_sensormessage_drive_train_creates_correct_sensormessage(self, drive_train_scene):
        robots = find_robots_in_scene(drive_train_scene)
        message = MessageFactory.sensor_message_from_objects(robots, 1.0)
        assert str(message) == _drive_train_facit

    def test_that_reading_position_controlmessage_updates_robots(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        controlmessage = create_faked_controlmessage_for(robots)

        update_robots_from_message(robots, controlmessage)

        assert "".join(str(robot) for robot in robots) == _updated_robots_same_input_types_facit

    def test_that_reading_multiple_input_type_controlmessage_updates_robots(self, scene_position_velocity_force_input):
        robots = find_robots_in_scene(scene_position_velocity_force_input)
        controlmessage = create_faked_controlmessage_persignal_for(robots)
        update_robots_from_message(robots, controlmessage)

        # Explanation next line: Because we have not taken a simulation step, there is no output from GetData
        robots[1].input_signals[1].GetData = lambda: robots[1].input_signals[1].Motor.ConstantForce
        assert robots[1].input_signals[1].GetData() == 5.2

        assert "".join(str(robot) for robot in robots) == _updated_robots_multiple_input_types_facit

    def data_to_float(self, arr):
        res = list()
        for val in arr:
            res.append(val.GetData())
        return res

    def data_to_float_motorforce(self, arr):
        """
        Retrieve set force without taking a simulation step (there is no output from GetData)
        # Ref: https://git.algoryx.se/algoryx/agx/-/issues/1101
        """
        res = list()
        for val in arr:
            res.append(val.Motor.ConstantForce)
        return res

    def test_that_reading_force_controlmessage_updates_robots(self, scene_forceinput):
        robots = find_robots_in_scene(scene_forceinput)
        controlmessage = create_faked_controlmessage_for(robots)
        update_robots_from_message(robots, controlmessage)
        assert self.data_to_float_motorforce(robots[0].input_signals) == [3, 6]

    def test_that_reading_velocity_controlmessage_updates_robots(self, scene_velocityinput):
        robots = find_robots_in_scene(scene_velocityinput)
        controlmessage = create_faked_controlmessage_for(robots)
        update_robots_from_message(robots, controlmessage)
        assert self.data_to_float(robots[0].input_signals) == [2, 4]

    def test_that_reading_controlmessage_without_controlevent_skips_controlevent(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        assert robots[0].control_events()['gripper'].GetData() == 200.0
        assert robots[1].control_events()['gripper'].GetData() == 200.0
        controlmessage = create_faked_controlmessage_for(robots, add_control_event=False)
        update_robots_from_message(robots, controlmessage)
        assert robots[0].control_events()['gripper'].GetData() == 200.0
        assert robots[1].control_events()['gripper'].GetData() == 200.0

    @pytest.mark.parametrize("the_scene", ["scene_positioninput", "scene_velocityinput", "scene_forceinput"])
    def test_that_missing_values_in_controlmessage_gives_informative_exception(self, the_scene, request):
        robots = find_robots_in_scene(request.getfixturevalue(the_scene))
        robot0 = [robots[0]]
        bad_controlmessage = create_faked_controlmessage_for(robot0)
        # Add robot1 with no values
        bad_controlmessage.objects[robots[1].name]
        with pytest.raises(AssertionError) as excinfo:
            update_robots_from_message(robots, bad_controlmessage)
        assert "Mismatching number of values for robot2 in controlmessage, got 0 should be 2" in str(excinfo)

    def test_that_missing_robot_in_controlmessage_gives_informative_exception(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        robot0 = [robots[0]]
        bad_controlmessage = create_faked_controlmessage_for(robot0)
        with pytest.raises(AssertionError) as excinfo:
            update_robots_from_message(robots, bad_controlmessage)
        assert "Robot robot2 not found in controlmessage - client probably sent wrong name" in str(excinfo)


@pytest.mark.integrationtest
class Test_sensor_message_from_objects:

    def test_that_generating_sensormessage_creates_correct_sensormessage(self, scene_positioninput):
        robots = find_robots_in_scene(scene_positioninput)
        message = MessageFactory.sensor_message_from_objects(robots, 1.0)
        assert str(message) == _sensor_facit

    def test_that_click_box_is_included_in_sensormessage(self, click_scene):
        objects = get_click_configuration(click_scene)

        message = MessageFactory.sensor_message_from_objects(objects, 1.0)

        assert message.objects["box"].objectSensors[0].position.arr == [1, 2, 3]
        assert message.objects["box"].objectSensors[1].rpy.arr == approx([2, 1, 1], 1e-3)

    def test_that_sensors_are_included_in_sensormessage(self, sensor_scene):
        robots = find_robots_in_scene(sensor_scene)

        message = MessageFactory.sensor_message_from_objects(robots, 1.0)

        assert message.objects["robot"].sensors["forceTorqueSensor"].sensor[0].force.arr == [0, 0, 0]
        assert message.objects["robot"].sensors["forceTorqueSensor"].sensor[1].directionalTorque.arr == [0, 0, 0]

    def test_that_drive_train_sensors_are_included_in_sensormessage(self, drive_train_scene):
        robots = find_robots_in_scene(drive_train_scene)

        message = MessageFactory.sensor_message_from_objects(robots, 1.0)

        assert message.objects["robot"].sensors["engineAngle"].sensor[0].angle == 0.0
        assert message.objects["robot"].sensors["engineTorque"].sensor[0].torque == 0.0
        assert message.objects["robot"].sensors["engineVelocity"].sensor[0].angularVelocity == 0.0


_handshake_facit = """messageType: HandshakeMessageType
version: CURRENT_VERSION
objects {
  key: "robot2"
  value {
    controlsInOrder: "robot2_joint0"
    controlsInOrder: "robot2_joint1"
    controlTypesInOrder: Angle
    controlTypesInOrder: Angle
    jointSensorsInOrder: "robot2_joint0"
    jointSensorsInOrder: "robot2_joint1"
    jointSensors: Angle
    jointSensors: AngularVelocity
    jointSensors: Torque
    controlEvents {
      key: "gripper"
      value: Activated
    }
    objectSensors: Position
    objectSensors: RPY
  }
}
objects {
  key: "robot1"
  value {
    controlsInOrder: "robot1_joint0"
    controlsInOrder: "robot1_joint1"
    controlTypesInOrder: Angle
    controlTypesInOrder: Angle
    jointSensorsInOrder: "robot1_joint0"
    jointSensorsInOrder: "robot1_joint1"
    jointSensors: Angle
    jointSensors: AngularVelocity
    jointSensors: Torque
    controlEvents {
      key: "gripper"
      value: Activated
    }
    objectSensors: Position
    objectSensors: RPY
  }
}
simSettings {
  timeStep: 0.03
}
"""


_sensor_facit = """messageType: SensorMessageType
objects {
  key: "robot2"
  value {
    angleSensors: 0
    angleSensors: 0
    angularVelocitySensors: 0
    angularVelocitySensors: 0
    torqueSensors: 0
    torqueSensors: 0
    objectSensors {
      position {
        arr: 0
        arr: -0.45
        arr: 0.45
      }
    }
    objectSensors {
      rpy {
        arr: 0
        arr: 0
        arr: -0.75
      }
    }
  }
}
objects {
  key: "robot1"
  value {
    angleSensors: 0
    angleSensors: 0
    angularVelocitySensors: 0
    angularVelocitySensors: 0
    torqueSensors: 0
    torqueSensors: 0
    objectSensors {
      position {
        arr: -0.25
        arr: 3.45
        arr: 0.55
      }
    }
    objectSensors {
      rpy {
        arr: 0
        arr: 0
        arr: 2.35
      }
    }
  }
}
simVars {
  simulatedTime: 1
}
"""

_drive_train_facit = """messageType: SensorMessageType
objects {
  key: "robot"
  value {
    angleSensors: 0
    angleSensors: 0
    angularVelocitySensors: 0
    angularVelocitySensors: 0
    torqueSensors: 0
    torqueSensors: 0
    objectSensors {
      position {
        arr: 0
        arr: 0
        arr: 0
      }
    }
    objectSensors {
      rpy {
        arr: 0
        arr: 0
        arr: 0
      }
    }
    sensors {
      key: "engineVelocity"
      value {
        sensor {
          angularVelocity: 0
        }
        sensor {
          angularVelocity: 0
        }
      }
    }
    sensors {
      key: "engineTorque"
      value {
        sensor {
          torque: 0
        }
        sensor {
          torque: 0
        }
      }
    }
    sensors {
      key: "engineAngle"
      value {
        sensor {
          angle: 0
        }
        sensor {
          angle: 0
        }
      }
    }
  }
}
simVars {
  simulatedTime: 1
}
"""

_updated_robots_same_input_types_facit = """name: robot1
    num_joints: 2
    jointnames: ['robot1_joint0', 'robot1_joint1']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    2 input_types: ['Brick.Signal.LockPositionInput', 'Brick.Signal.LockPositionInput']
    2 input_values: [57.29577951308232, 114.59155902616465]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['gripper: 0.0']
name: robot2
    num_joints: 2
    jointnames: ['robot2_joint0', 'robot2_joint1']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    2 input_types: ['Brick.Signal.LockPositionInput', 'Brick.Signal.LockPositionInput']
    2 input_values: [57.29577951308232, 114.59155902616465]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['gripper: 100.0']
"""

_updated_robots_multiple_input_types_facit = """name: robot1
    num_joints: 2
    jointnames: ['robot1_joint0', 'robot1_joint1']
    control input type: Multiple
    2 input_types: ['Brick.Signal.LockPositionInput', 'Brick.Signal.MotorVelocityInput']
    2 input_values: [74.48451336700703, 2.6]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['gripper: 0.0']
name: robot2
    num_joints: 2
    jointnames: ['robot2_joint0', 'robot2_joint1']
    control input type: Multiple
    2 input_types: ['Brick.Signal.LockPositionInput', 'Brick.Signal.MotorForceInput']
    2 input_values: [223.45354010102108, 5.2]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['gripper: 100.0']
"""
