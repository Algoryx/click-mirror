from brick_env import BrickEnv
import pytest


@pytest.mark.integrationtest
def test_that_brick_env_enables_loading_robots(pyroot):

    file_path = pyroot + "/testdata/MyRobot.yml"
    model_name = "MyRobotPosition"

    brickenv = BrickEnv()
    scene = brickenv.load_from_file(file_path, model_name)

    import Brick.Robotics
    assert scene.__class__ is Brick.Robotics.Robot
    assert len(scene['InternalComponents']) == 4
