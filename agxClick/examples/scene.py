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


def createArgumentParser(args):
    parser = argparse.ArgumentParser(args)
    parser.add_argument('--brickfile', type=str, default="", help="The path to the brick .yml file")
    parser.add_argument('--model', type=str, default="", help="The name of the Physics.Component to be loaded from brickfile")
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
    logging.getLogger(__file__).info(f"Loading {args.brickfile}:{args.model}")
    scene = Component.CreateFromFile(args.brickfile, args.model)
    BrickSimulation.Default.AddComponent(scene)
    agxOSG.createVisual(sim, app.getSceneRoot())
