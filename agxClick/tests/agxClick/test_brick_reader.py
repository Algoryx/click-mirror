import pytest
from agxClick import find_robots_in_scene, get_click_configuration, has_click_configuration
from os import chdir


@pytest.mark.integrationtest
class Test_click_brick_reader:

    @pytest.fixture(scope="class", autouse=True)
    def exec_from_brick_config_expected_path(self, request):
        print(f"Changing exec directory from {request.config.invocation_dir} to {request.fspath.dirname}/..")
        chdir(f"{request.fspath.dirname}/..")
        yield
        chdir(request.config.invocation_dir)

    def test_that_model_has_robots_with_arms(self, scene):
        import Brick.Robotics
        assert len(scene['InternalComponents']) == 12, f"All recursive components not found"
        assert scene['InternalComponents'][0].__class__ is Brick.Robotics.Robot
        assert len(scene['InternalComponents'][0].Arms) == 1
        assert len(scene['InternalComponents'][0].Arms[0].Joints) == 2

    def test_that_robot_has_jointnames(self, scene):
        robots = find_robots_in_scene(scene)
        assert len(robots[0].brickrobot.Arms[0].Joints) == 2
        print(robots[0].brickrobot.Arms[0].Joints[0].ProtocolReference)
        assert "robot1_joint0" in robots[0].joint_protocolrefs()
        assert "robot2_joint1" in robots[1].joint_protocolrefs()
        assert len(robots[0].joint_protocolrefs()) == 2

    def test_that_robot_has_velocity_input(self, scene_velocityinput):
        robots = find_robots_in_scene(scene_velocityinput)
        robots[0].validate()
        import Brick.Signal
        assert robots[0].controlType() == Brick.Signal.MotorVelocityInput

    def test_that_robot_has_force_input(self, scene_forceinput):
        robots = find_robots_in_scene(scene_forceinput)
        robots[0].validate()
        import Brick.Signal
        assert robots[0].controlType() == Brick.Signal.MotorForceInput

    def test_that_robot_has_position_input(self, scene):
        robots = find_robots_in_scene(scene)
        robots[0].validate()
        import Brick.Signal
        assert robots[0].controlType() == Brick.Signal.LockPositionInput

    def test_that_robot_has_gripper(self, scene):
        robots = find_robots_in_scene(scene)
        assert robots[0].has_control_events()
        import Brick.Signal
        assert robots[0].control_events()["adhesiveForceInput"].__class__ == Brick.Signal.AdhesiveForceInput
        assert robots[0].control_events()["adhesiveForceInput"].GetData() == 200.0
        assert robots[1].control_events()["adhesiveForceInput"].GetData() == 200.0

    def test_that_robot_grippers_are_enabled(self, scene):
        robots = find_robots_in_scene(scene)
        assert robots[0].control_events()["adhesiveForceInput"].GetData() == 200.0
        assert robots[1].control_events()["adhesiveForceInput"].GetData() == 200.0

    def test_that_robot_has_pose(self, scene):
        import Brick.Math
        robots = find_robots_in_scene(scene)
        assert robots[0].brickrobot.Transform.Position.__class__ == Brick.Math.Vec3
        assert robots[0].brickrobot.Transform.Rotation.__class__ == Brick.Math.Quat

    def test_that_model_has_expected_topology(self, scene):
        robots = find_robots_in_scene(scene)
        assert len(robots) == 2
        assert len(robots[0].input_signals) == 2
        assert str(robots[0]) == """name: robot1
    num_joints: 2
    jointnames: ['robot1_joint0', 'robot1_joint1']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    2 input_signals: Brick.Signal.LockPositionInput: [0.0, 0.0]
    2 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0]
    2 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0]
    2 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0]
    control events: ['adhesiveForceInput: 200.0']
"""

    def test_that_brick_config_is_read(self, click_scene):
        assert has_click_configuration(click_scene)
        objects = get_click_configuration(click_scene)
        assert len(objects) == 3

    def test_that_scene_does_not_have_click_configuration(self, scene):
        assert not has_click_configuration(scene)

    def test_that_exception_raised_for_missing_protocolreference(self, scene_missing_protref):
        with pytest.raises(AssertionError) as excinfo:
            find_robots_in_scene(scene_missing_protref)
        assert "Missing protocolReference in robot robot1" in str(excinfo)

    def test_that_robot_has_force_and_torque_sensor(self, sensor_scene):
        import Brick.Signal
        robots = find_robots_in_scene(sensor_scene)
        assert Brick.Signal.ForceVectorOutput == robots[0].sensors["force-sensor"][0].__class__
        assert Brick.Signal.TorqueVectorOutput == robots[0].sensors["torque-sensor"][0].__class__
