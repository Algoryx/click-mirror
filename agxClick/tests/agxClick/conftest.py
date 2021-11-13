import pytest
from brick_env import BrickEnv
import os


@pytest.fixture(scope="session")
def brickenv():
    return BrickEnv()


@pytest.fixture(scope="session")
def pyroot():
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
