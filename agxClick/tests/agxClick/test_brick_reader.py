import pytest
from clicktools import find_robots_in_scene, get_click_configuration, has_click_configuration
from os import chdir


@pytest.fixture(scope="class")
def scene(brickenv, pyroot):
    file_path = f"{pyroot}/tests/testdata/Example5.yml"
    model_name = "Example5"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="class")
def scene_velocityinput(brickenv, pyroot):
    file_path = f"{pyroot}/tests/testdata/Example5.yml"
    model_name = "Example5Velocity"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="class")
def scene_forceinput(brickenv, pyroot):
    file_path = f"{pyroot}/tests/testdata/Example5.yml"
    model_name = "Example5Force"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="function")
def clickscene(brickenv, pyroot):
    file_path = f"{pyroot}/tests/testdata/ClickScene.yml"
    model_name = "ExampleClickScene"
    return brickenv.load_from_file(file_path, model_name)


@pytest.mark.linuxcrash
class Test_click_brick_reader:

    @pytest.fixture(scope="class", autouse=True)
    def exec_from_brick_config_expected_path(self, request):
        print(f"Changing exec directory from {request.config.invocation_dir} to {request.fspath.dirname}/..")
        chdir(f"{request.fspath.dirname}/..")
        yield
        chdir(request.config.invocation_dir)

    def test_that_model_has_robots_with_arms(self, scene):
        assert len(scene['InternalComponents']) == 2
        assert "Robot" in str(scene['InternalComponents'][1]._ModelType.Origin)
        assert len(scene['InternalComponents'][1].Arms) == 1
        assert len(scene['InternalComponents'][1].Arms[0].Joints) == 7

    def test_that_robot_has_jointnames(self, scene):
        robots = find_robots_in_scene(scene)
        assert "panda_joint1" in robots[0].joint_externalrefs()
        assert "panda_joint7" in robots[0].joint_externalrefs()
        assert len(robots[0].joint_externalrefs()) == 7

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
        assert len(robots[0].input_signals) == 7
        assert str(robots[0]) == """name: panda_tool
    num_joints: 7
    jointnames: ['panda_joint1', 'panda_joint2', 'panda_joint3', 'panda_joint4', 'panda_joint5', 'panda_joint6', 'panda_joint7']
    control input type: <class 'Brick.Signal.LockPositionInput'>
    7 input_signals: Brick.Signal.LockPositionInput: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 45.0]
    7 torque_sensors: Brick.Signal.LockForceOutput: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    7 angle_sensors: Brick.Signal.MotorAngleOutput: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    7 velocity_sensors: Brick.Signal.MotorVelocityOutput: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    control events: ['adhesiveForceInput: 200.0']
"""

    def test_that_brick_config_is_read(self, clickscene):
        assert has_click_configuration(clickscene)
        objects = get_click_configuration(clickscene)
        assert len(objects) == 3

    def test_that_scene_does_not_have_click_configuration(self, scene):
        assert not has_click_configuration(scene)
