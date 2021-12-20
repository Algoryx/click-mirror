import pytest
from brick_env import BrickEnv
import os


@pytest.fixture(scope="session")
def brickenv():
    return BrickEnv()


@pytest.fixture(scope="session")
def pyroot():
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


@pytest.fixture(scope="function")
def scene(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/MyScene.yml"
    model_name = "MyScenePosition"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="class")
def scene_velocityinput(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/MyScene.yml"
    model_name = "MySceneVelocity"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="class")
def scene_forceinput(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/MyScene.yml"
    model_name = "MySceneForce"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="class")
def scene_missing_protref(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/MissingProtRefScene.yml"
    model_name = "MyScene"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="function")
def click_scene(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/ClickScene.yml"
    model_name = "ExampleClickScene"
    return brickenv.load_from_file(file_path, model_name)


@pytest.fixture(scope="function")
def sensor_scene(brickenv, pyroot):
    file_path = f"{pyroot}/testdata/ClickScene.yml"
    model_name = "ExampleSensorClickScene"
    return brickenv.load_from_file(file_path, model_name)
