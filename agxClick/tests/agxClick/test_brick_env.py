from brick_env import BrickEnv
import pytest


@pytest.mark.integrationtest
def test_brick_load_robots(pyroot):

    file_path = pyroot + "/testdata/Example5.yml"
    model_name = "Example5"

    brickenv = BrickEnv()
    scene = brickenv.load_from_file(file_path, model_name)

    assert len(scene['InternalComponents']) == 3
    assert "Robot" in str(scene['InternalComponents'][1]._ModelType.Origin)
    print(type(scene['InternalComponents'][1]))
    assert len(scene['InternalComponents'][1].Arms) == 1
