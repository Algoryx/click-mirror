"""
Example scene.py, constructed so that it can either be used with
python3 click_application --brickfile <brickfile>
or
agxViewer scene.py --brickfile <brickfile>
"""
import agxOSG
from agxPythonModules.utils.environment import simulation, application
import argparse
import logging
from typing import Tuple


def createArgumentParser(args):
    parser = argparse.ArgumentParser(args)
    parser.add_argument('--model', type=str, default="", help="<file.yml>[:component] The path to the brick .yml file and the name of the Physics.Component to be loaded from it")
    args, _ = parser.parse_known_args(args)
    return args


def buildScene():
    app = application()
    sim = simulation()
    arguments = app.getArguments()
    argument_string = [arguments.getArgumentName(a) for a in range(2, arguments.getNumArguments())]
    args = createArgumentParser(argument_string)

    from Brick.Physics import Component
    from Brick.AGXBrick import BrickSimulation
    file, model = extract_file_model(args.model)
    logging.getLogger(__file__).info(f"Loading {file}:{model}")
    scene = Component.CreateFromFile(file, model)
    BrickSimulation.Default.AddComponent(scene)
    agxOSG.createVisual(sim, app.getSceneRoot())
    return scene


def extract_file_model(file_model: str) -> Tuple[str]:
    return file_model.rsplit(":", 1)
