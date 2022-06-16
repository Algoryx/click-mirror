import pytest
from brick_env import BrickEnv
import os


@pytest.fixture(scope="session")
def session_brickenv():
    return BrickEnv()


# We empty the simulation instead of creating a new one
@pytest.fixture()
def brickenv(session_brickenv):
    yield session_brickenv
    session_brickenv.cleanup()


# This one is slower and not used here, because we have to init brick for every test. Better isolation though.
@pytest.fixture()
def brickenv_deinited():
    brick_env = BrickEnv()
    yield brick_env
    brick_env.deinit()


# Note function scope.
# We must have function scope on fixtures anyway when using Brick to ensure isolation since Brick is stateful, i.e.
# Brick will overlay models on top of eachother if not cleaned
@pytest.fixture
def load_brick_scene(brickenv, pyroot):
    """
    Loads a rel_file_path from pyroot and model_name using brick
    """

    def inner(rel_file_path: str, model_name: str):
        return brickenv.load_from_file(f"{pyroot}/{rel_file_path}", model_name)

    return inner


@pytest.fixture(scope="session")
def pyroot():
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


@pytest.fixture
def example5(load_brick_scene):
    return load_brick_scene("/tests/testdata/Example5.yml", "Example5")


@pytest.fixture(scope="function")
def scene_positioninput(load_brick_scene):
    return load_brick_scene("/testdata/MyScene.yml", "MyScenePosition")


@pytest.fixture(scope="function")
def scene_velocityinput(load_brick_scene):
    return load_brick_scene("/testdata/MyScene.yml", "MySceneVelocity")


@pytest.fixture(scope="function")
def scene_forceinput(load_brick_scene):
    return load_brick_scene("/testdata/MyScene.yml", "MySceneForce")


@pytest.fixture(scope="function")
def scene_missing_protref(load_brick_scene):
    return load_brick_scene("/testdata/MissingProtRefScene.yml", "MyScene")


@pytest.fixture(scope="function")
def click_scene(load_brick_scene):
    return load_brick_scene("/testdata/ClickScene.yml", "ExampleClickScene")


@pytest.fixture(scope="function")
def sensor_scene(load_brick_scene):
    return load_brick_scene("/testdata/ClickScene.yml", "ExampleSensorClickScene")


@pytest.fixture(scope="function")
def drive_train_scene(load_brick_scene):
    return load_brick_scene("/testdata/ClickScene.yml", "ExampleDriveTrainClickScene")
