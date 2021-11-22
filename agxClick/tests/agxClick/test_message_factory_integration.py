from pClick.Messaging_pb2 import Force
import pytest
from agxClick import MessageFactory, update_robots_from_message, find_robots_in_scene, get_click_configuration, ClickRobot, ClickObject
from pClick import ValueType, MessageFactory as ProtoMessageFactory
from typing import List
from os import chdir
from pytest import approx


def create_faked_controlmessage_for(robots: List[ClickRobot], add_control_event=True):
    """
    add_control_event, if True an event will be added with False for first robot, True for rest
    """
    control_m = ProtoMessageFactory.create_controlmessage()
    for i, robot in enumerate(robots):
        control = control_m.objects[robot.name]
        controltype = MessageFactory.to_click_control_type(robot.controlType())
        if controltype == ValueType.Angle:
            control.angles.extend([x * 1.0 for x in range(1, robot.num_joints + 1)])
        elif controltype == ValueType.AngleVelocity:
            control.angleVelocities.extend([x * 2.0 for x in range(1, robot.num_joints + 1)])
        elif controltype == ValueType.Torque:
            control.torques.extend([x * 3.0 for x in range(1, robot.num_joints + 1)])
        else:
            raise Exception(f"Faking {robot.controlType()} has not been implemented")
        if (add_control_event):
            for name in robot.control_events().keys():
                control.controlEvents[name] = i
    return control_m


@pytest.mark.integrationtest
class Test_message_factory_integration:

    @pytest.fixture(scope="class", autouse=True)
    def exec_from_brick_config_expected_path(self, request):
        print(f"Changing exec directory from {request.config.invocation_dir} to {request.fspath.dirname}/..")
        chdir(f"{request.fspath.dirname}/..")
        yield
        chdir(request.config.invocation_dir)

    def test_joints(self, scene):
        robots = find_robots_in_scene(scene)
        assert [x * 1.0 for x in range(1, robots[0].num_joints + 1)] == [1.0, 2.0]

    def test_that_generating_handshake_creates_correct_handshake(self, scene):
        robots = find_robots_in_scene(scene)
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert str(message) == _handshake_facit

    def test_that_generating_handshake_creates_correct_handshake_velocity_input(self, scene):
        robots = find_robots_in_scene(scene)
        import Brick.Signal
        robots[0].input_signals = [Brick.Signal.MotorVelocityInput()] * len(robots[0].input_signals)
        robots[0].validate()
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert message.controlType == ValueType.AngleVelocity

    def test_that_generating_handshake_creates_correct_handshake_force_input(self, scene):
        robots = find_robots_in_scene(scene)
        import Brick.Signal
        robots[0].input_signals = [Brick.Signal.MotorForceInput()] * len(robots[0].input_signals)
        robots[0].validate()
        message = MessageFactory.handshake_message_from_objects(robots, 0.03)
        assert message.controlType == ValueType.Torque

    def test_that_generating_sensormessage_creates_correct_sensormessage(self, scene):
        robots = find_robots_in_scene(scene)
        message = MessageFactory.sensor_message_from_objects(robots, 1.0)
        assert str(message) == _sensor_facit

    def test_that_reading_position_controlmessage_updates_robots(self, scene):
        robots = find_robots_in_scene(scene)
        controlmessage = create_faked_controlmessage_for(robots)
        update_robots_from_message(robots, controlmessage)
        assert "".join(str(robot) for robot in robots) == _updated_robots_str

    def data_to_float(self, arr):
        res = list()
        for val in arr:
            res.append(val.GetData())
        return res

    def data_to_float_motorforce(self, arr):
        res = list()
        for val in arr:
            res.append(val.Motor.MaxForce)
        return res

    def test_that_reading_force_controlmessage_updates_robots(self, scene_forceinput):
        robots = find_robots_in_scene(scene_forceinput)
        controlmessage = create_faked_controlmessage_for(robots)
        update_robots_from_message(robots, controlmessage)
        # TODO: When agxBrick with issue 1101 fixed is released, use data_to_float, it will implement GetData correctly
        # Ref: https://git.algoryx.se/algoryx/agx/-/issues/1101
        assert self.data_to_float_motorforce(robots[0].input_signals) == [3, 6]

    def test_that_reading_velocity_controlmessage_updates_robots(self, scene_velocityinput):
        robots = find_robots_in_scene(scene_velocityinput)
        controlmessage = create_faked_controlmessage_for(robots)
        update_robots_from_message(robots, controlmessage)
        assert self.data_to_float(robots[0].input_signals) == [2, 4]

    def test_that_reading_controlmessage_without_controlevent_skips_controlevent(self, scene):
        robots = find_robots_in_scene(scene)
        assert robots[0].control_events()['adhesiveForceInput'].GetData() == 200.0
        assert robots[1].control_events()['adhesiveForceInput'].GetData() == 200.0
        controlmessage = create_faked_controlmessage_for(robots, add_control_event=False)
        update_robots_from_message(robots, controlmessage)
        assert robots[0].control_events()['adhesiveForceInput'].GetData() == 200.0
        assert robots[1].control_events()['adhesiveForceInput'].GetData() == 200.0

    def test_that_missing_values_in_controlmessage_gives_informative_exception(self, scene, scene_velocityinput, scene_forceinput):
        for s in [scene, scene_velocityinput, scene_forceinput]:
            robots = find_robots_in_scene(s)
            missing_robot = [robots[0]]
            bad_controlmessage = create_faked_controlmessage_for(missing_robot)
            with pytest.raises(AssertionError) as excinfo:
                update_robots_from_message(robots, bad_controlmessage)
            assert "Missing values for robot2 in controlmessage, got 0/2" in str(excinfo)

    def test_that_click_box_is_included_in_handshake(self, clickscene):
        objects = get_click_configuration(clickscene)

        message = MessageFactory.handshake_message_from_objects(objects, 0.03)

        assert ValueType.Position in message.objects["box"].objectSensors
        assert ValueType.RPY in message.objects["box"].objectSensors
        assert message.objects["box"].objectSensors == [ValueType.Position, ValueType.RPY]

    def test_that_click_box_is_included_in_sensormessage(self, clickscene):
        objects = get_click_configuration(clickscene)

        message = MessageFactory.sensor_message_from_objects(objects, 1.0)

        assert message.objects["box"].objectSensors[0].position.arr == [1, 2, 3]
        assert message.objects["box"].objectSensors[1].rpy.arr == approx([2, 1, 1], 1e-3)


_handshake_facit = """messageType: HandshakeMessageType
version: CURRENT_VERSION
objects {
  key: "robot1"
  value {
    controlsInOrder: "robot1_joint0"
    controlsInOrder: "robot1_joint1"
    jointSensors: Angle
    jointSensors: AngleVelocity
    jointSensors: Force
    controlEvents {
      key: "adhesiveForceInput"
      value: Activated
    }
    objectSensors: Position
    objectSensors: RPY
    jointSensorsInOrder: "robot1_joint0"
    jointSensorsInOrder: "robot1_joint1"
  }
}
objects {
  key: "robot2"
  value {
    controlsInOrder: "robot2_joint0"
    controlsInOrder: "robot2_joint1"
    jointSensors: Angle
    jointSensors: AngleVelocity
    jointSensors: Force
    controlEvents {
      key: "adhesiveForceInput"
      value: Activated
    }
    objectSensors: Position
    objectSensors: RPY
    jointSensorsInOrder: "robot2_joint0"
    jointSensorsInOrder: "robot2_joint1"
  }
}
simSettings {
  timeStep: 0.03
}
"""

_sensor_facit = """messageType: SensorMessageType
objects {
  key: "robot1"
  value {
    angleSensors: 0.0
    angleSensors: 0.0
    angleVelocitySensors: 0.0
    angleVelocitySensors: 0.0
    torqueSensors: 0.0
    torqueSensors: 0.0
    objectSensors {
      position {
        arr: -0.25
        arr: 3.45
        arr: 0.55
      }
    }
    objectSensors {
      rpy {
        arr: 0.0
        arr: 0.0
        arr: 2.35
      }
    }
  }
}
objects {
  key: "robot2"
  value {
    angleSensors: 0.0
    angleSensors: 0.0
    angleVelocitySensors: 0.0
    angleVelocitySensors: 0.0
    torqueSensors: 0.0
    torqueSensors: 0.0
    objectSensors {
      position {
        arr: 0.0
        arr: -0.45
        arr: 0.45
      }
    }
    objectSensors {
      rpy {
        arr: 0.0
        arr: 0.0
        arr: -0.75
      }
    }
  }
}
simVars {
  simulatedTime: 1.0
}
"""

_updated_robots_str = """name: robot1
    num_joints: 2
    jointnames: ['robot1_joint0', 'robot1_joint1']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    2 input_signals: Brick.Signal.LockPositionInput: [57.29577951308232, 114.59155902616465]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['adhesiveForceInput: 0.0']
name: robot2
    num_joints: 2
    jointnames: ['robot2_joint0', 'robot2_joint1']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    2 input_signals: Brick.Signal.LockPositionInput: [57.29577951308232, 114.59155902616465]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['adhesiveForceInput: 100.0']
"""
