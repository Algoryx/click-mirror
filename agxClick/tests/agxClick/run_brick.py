from brick_env import BrickEnv
import os


pyroot = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
file_path = pyroot + "/models/Example5.yml"
model_name = "Example5"

brickenv = BrickEnv()
scene = brickenv.load_from_file(file_path, model_name)

assert len(scene['InternalComponents']) == 3
assert "Robot" in str(scene['InternalComponents'][1]._ModelType.Origin)
print(type(scene['InternalComponents'][1]))
assert len(scene['InternalComponents'][1].Arms) == 1
