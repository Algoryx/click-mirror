import pytest
from brick_env import BrickEnv


@pytest.fixture(scope="session")
def brickenv():
    return BrickEnv()
