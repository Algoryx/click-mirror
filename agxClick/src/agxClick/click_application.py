from agxClick import KeyboardListener, AgxApplication, ClickFrameListener
from agxClick.click_event_listener import _REGISTER_FRAME_LISTENER
from typing import Any, Callable, List
import logging
import time


class WallClock:

    def __init__(self):
        self.time = time.time()

    def time_since_init(self):
        return time.time() - self.time

    def timestamp(self):
        return time.time()

    def time_since(self, timestamp: float) -> float:
        return time.time() - timestamp


class ClickApplication(AgxApplication):
    _click_frame_listener: ClickFrameListener
    _keyboardListener: KeyboardListener

    def __init__(self, args: List[str]):
        super().__init__(args)
        self._logger = logging.getLogger(__file__)
        self._stop_application = False
        self.args = self.parse_arguments(args)

    def run(self, buildScene: Callable[[], Any]):
        """
        buildScene must return the top Brick.Physics.Component of the scene
        """
        self.add_listeners()
        scene = buildScene()
        self.update_listeners(scene)

        # Store setting so we can restore it if overridden
        self._realtime_sync = self.app.getRealTimeSync()
        # We must control stepping ourselves to be able to drive steps from controller, and sync realtime if wanted
        self.app.setAutoStepping(False)

        self._logger.info(f"Entering main loop {'with' if self.app.getViewer() else 'without'} graphics")
        self._logger.info(f"Realtime sync is {self.app.getRealTimeSync()}")

        num_frames, wall_clock = self.mainloop()

        self.report_timing(num_frames, wall_clock)

    def mainloop(self):
        num_frames = 0
        while not self._stop_application:
            if not self._click_frame_listener.handshake_completed:
                wall_clock = WallClock()

            if self._click_frame_listener.step_simulation():
                self.app.step()
            self.stepApplication()
            if not _REGISTER_FRAME_LISTENER:
                self._click_frame_listener.preFrame(self.sim.getClock().getTime())
            self.enforce_step_realtime_settings()
            if self.args.stopAfterFrame and self.sim.getClock().getFrame() >= self.args.stopAfterFrame:
                self._stop_application = True
            if self.args.stopAfter and self.sim.getClock().getTime() >= self.args.stopAfter:
                self._stop_application = True
            num_frames = num_frames + 1

        return num_frames, wall_clock

    def report_timing(self, num_frames, wall_clock):
        simsteps = self.sim.getClock().getFrame()
        walltime = wall_clock.time_since_init()
        self._logger.info(f"Executed {num_frames} frames and {simsteps} simulation steps")
        self._logger.info(f"simulated time: {self.sim.getClock().getTime()} Wall clock time: {walltime}")
        return simsteps, walltime

    def parse_arguments(self, args):
        from argparse import ArgumentParser
        from distutils.util import strtobool
        parser = ArgumentParser(args)
        parser.add_argument('--stopAfterFrame', type=int, default=None, help="Stop when this number of simulation are executed")
        parser.add_argument('--stopAfter', type=float, default=None, help="Stop when this simulation time is reached")
        args, _ = parser.parse_known_args(args)
        return args

    def on_exception(self, ex: Exception):
        self._stop_application = True
        self._logger.warning(f"on_exception called with {ex}")

    def on_stop(self):
        self._stop_application = True
        self.stop()
        self._click_frame_listener.stop()

    def on_keyboard_reset(self):
        self.reset_scene(self._scene)
        self._click_frame_listener.send_reset()

    def on_reset_message(self):
        assert self._scene
        self.reset_scene(self._scene)

    def update_listeners(self, scene):
        """
        We need to add listeners before possible listeners added by buildScene, which is why these updates are needed
        """
        self._scene = scene
        self._click_frame_listener.update_scene(scene)

    def add_listeners(self):
        self._click_frame_listener = ClickFrameListener(scene=None, app=self.app,
                                                        on_stop=self.on_stop,
                                                        on_exception=self.on_exception,
                                                        on_reset=self.on_reset_message)
        if _REGISTER_FRAME_LISTENER:
            self.app.addListener(self._click_frame_listener)

        self._keyboardListener = KeyboardListener(on_stop=self.on_stop,
                                                  on_reset=self.on_keyboard_reset)
        self.sim.add(self._keyboardListener)

    def enforce_step_realtime_settings(self):
        if self.app.getAutoStepping():
            self._logger.info(f"Something (SimulationView?) enabled autostepping, disabling it again...")
            self.app.setAutoStepping(False)
        if self.app.getRealTimeSync() is not self._realtime_sync:
            self._logger.info(f"Something (SimulationView?) changed realtime sync, forcing it to {self._realtime_sync} again...")
            self.app.setRealTimeSync(self._realtime_sync)
