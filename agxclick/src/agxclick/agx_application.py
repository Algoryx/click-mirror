import agx
import agxOSG
import agxIO
import agxSDK
from typing import List
from agxBrickHosting import registerAgxSimulation
from agxPythonModules.utils.environment import create_or_set_script_context


class AgxApplication:

    def __init__(self, args: List[str]):
        self.init = agx.AutoInit()
        self.app = agxOSG.ExampleApplication()
        self.app.init(agxIO.ArgumentParser(args))
        # After app.init() Brick can be imported
        from Brick.AGXBrick import BrickSimulation
        self.sim = agxSDK.Simulation()
        self.app.initSimulation(self.sim, True)
        registerAgxSimulation(self.sim, "default")
        assert BrickSimulation.GetSimulation("default"), "Could not find brick simulation Default"
        assert BrickSimulation.Default, "Could not find brick simulation Default via c# attribute"
        # create_or_set_script_context line makes environment.application() and simulation() work
        create_or_set_script_context(self.sim, self.app, None)
        self.m_timer = agx.HighAccuracyTimer(True)
        self.have_graphics = None

    def stepApplication(self):
        """
        Steps application
        - Calls Listeners like KeyboardListener and FrameListeners
        - Renders graphics
        - Send/Receives RCS
        """
        # Cache the fact that we have graphics or not. Depends on stepApplication not being called until after setting up agxOSG
        if self.have_graphics is None:
            self.have_graphics = self.app.getViewer()
        if self.have_graphics:
            self.app.executeOneStepWithGraphics(self.m_timer)
        else:
            self.app.executeOneStepWithoutGraphics()

    def stepSimulation(self):
        """
        Steps simulation one timestep
        If self.app.getRealTimeSync() then sleep rest of timeslot
        """
        self.app.step()

    def stop(self):
        self.app.stop()
        agx.Runtime.instance().clear()

    def remote_viewer_exists(self) -> bool:
        return hasattr(self.app, "getController") and self.app.getController()

    def reset_scene(self, scene_to_reset):
        from Brick.Physics import ComponentLoader
        from Brick.AGXBrick import BrickSimulation

        ComponentLoader.RepositionComponent(scene_to_reset)
        BrickSimulation.Default.ResetAgx()
        if self.remote_viewer_exists():
            self.app.pushFrameToRemoteViewer()
        BrickSimulation.Default.SyncOutputParameters()
