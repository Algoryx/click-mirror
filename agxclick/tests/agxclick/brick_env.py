import os
import agx
import agxSDK

from agxPythonModules.utils.environment import create_or_set_script_context

# agxBrickHosting corresponds to https://git.algoryx.se/algoryx/agx/-/blob/master/include/agxBrick/BrickInterop.h
from agxBrickHosting import (
    init as brick_init,
    registerAgxSimulation,
    unregisterAgxSimulation,
    getRegisteredAgxSimulation,
    locateBrickRootFromEnvironment,
)


class BrickEnv():
    '''
    Initializes agx and brick and provides utilities so you can do
    from brick_env import BrickEnv
    brickenv = BrickEnv()
    scene = brickenv.load_from_file(file_path, model_name)
    Inspired by https://git.algoryx.se/algoryx/agx/-/blob/master/brick/python/agxBrick/brick_gym_env.py
    '''

    @staticmethod
    def find_brick_sim_name(name):
        for i in range(5):
            try_name = name + str(i)
            if getRegisteredAgxSimulation(try_name) is None:
                return try_name
        raise Exception("Could not find an available name to register the AGX simulation")

    def __init__(self):
        super().__init__()
        self.init = agx.AutoInit()

        sim = agxSDK.Simulation()
        self.sim = sim
        sim.setUniformGravity(agx.Vec3(0.0, 0.0, -9.80665))

        # Initialize brick as in ExampleApplication::initBrickInterop
        brick_dir = locateBrickRootFromEnvironment()
        disable_python_net = os.getenv("BRICK_DISABLE_PYTHONNET") is None
        print(f"Brick dir: {brick_dir}")
        brick_init(brick_dir, "info", disable_python_net)

        self.brick_sim_name = self.find_brick_sim_name("env")
        registerAgxSimulation(sim, self.brick_sim_name)
        create_or_set_script_context(sim, None, None)

        from Brick.AGXBrick import BrickSimulation
        self.brickSimulation = BrickSimulation.GetSimulation(self.brick_sim_name)

    def load_from_file(self, file_path, model_name):
        from Brick.Physics import Component
        component = Component.CreateFromFile(file_path, model_name)
        self.brickSimulation.AddComponent(component)
        return component

    def deinit(self):
        """
        If using many BrickEnv, e.g. in testing, you need to call deinit when done to not run out of the 
        max 5 sim_names defined in find_brick_sim_name.

        Alternatively use cleanup for a faster solution
        """
        unregisterAgxSimulation(self.brick_sim_name)

    def cleanup(self):
        """
        If re-using the same BrickEnv, e.g. in testing, you need to call cleanup between usages to not have the
        new agx simulation loaded on top of the old one.

        Alternatively use deinit for a slower solution that creates a new simulation.
        """
        cleanup_mask = agxSDK.Simulation.SYSTEM + agxSDK.Simulation.SPACE
        self.brickSimulation.AgxSimulation.cleanup(cleanup_mask)
