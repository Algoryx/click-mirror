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
        self.simulation_stepping_enabled = self.args.start_paused is False
        self.disable_clicksync = self.args.disable_clicksync is True

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
        if self.disable_clicksync:
            self._logger.info("Simulation will not wait for controlmessages")
        else:
            self._logger.info("Pausing simulation until handshake is completed, then each controlmessage will step simulation once")

        if self.args.profile:
            import cProfile
            profile = cProfile.Profile()
            profile.enable()

        num_frames, wall_clock = self.mainloop()

        if self.args.profile:
            profile.disable()
            if self.args.profileFile == "":
                from pstats import SortKey, Stats
                ps = Stats(profile).sort_stats(SortKey.CUMULATIVE)
                ps.print_stats()
            else:
                profile.dump_stats(self.args.profileFile)

        self.report_timing(num_frames, wall_clock)

    def mainloop(self):
        num_frames = 0
        wall_clock = WallClock()
        click_sync_enabled = not self.disable_clicksync
        while not self._stop_application:
            # Reset wall_clock when handshake completed if syncing simulation step with controlmessage
            if click_sync_enabled and not self._click_frame_listener.handshake_completed:
                wall_clock = WallClock()

            if not _REGISTER_FRAME_LISTENER:
                self._click_frame_listener.preFrame(self.sim.getClock().getTime())
            click_ready_for_simstep = self.disable_clicksync or self._click_frame_listener.step_simulation()
            if click_ready_for_simstep and self.simulation_stepping_enabled:
                self.app.step()
            self.stepApplication()
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
        self._logger.info(f"Executed {num_frames} frames and {simsteps} simulation steps, received {self._click_frame_listener.num_controls_received} control messages")
        if (simsteps != self._click_frame_listener.num_controls_received):
            missed_simsteps = simsteps - self._click_frame_listener.num_controls_received
            self._logger.warn(f"Controller could not keep up with simulation! {missed_simsteps} simulation steps taken without control message")
        self._logger.info(f"simulated time: {self.sim.getClock().getTime()} Wall clock time: {walltime}")
        return simsteps, walltime

    def parse_arguments(self, args):
        from argparse import ArgumentParser
        parser = ArgumentParser(args)
        parser.add_argument('--stopAfterFrame', type=int, default=None, help="Stop when this number of simulation are executed")
        parser.add_argument('--stopAfter', type=float, default=None, help="Stop when this simulation time is reached")
        parser.add_argument('--startPaused', dest='start_paused', action="store_true", help="Start with simulation paused")
        parser.add_argument('--disableClickSync', dest='disable_clicksync', action="store_true", help="Do not sync each simulation step with click client - simulation will run withtout waiting for control messages")
        parser.add_argument('--profile', dest='profile', action="store_true", help="CProfile main loop and print results")
        parser.add_argument('--profileFile', type=str, default="", help="Write profile data to binary file (for snakeviz) instead of stdout")
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
        self._logger.info("Resetting scene")
        self.reset_scene(self._scene)
        self._click_frame_listener.send_reset()

    def on_reset_message(self):
        assert self._scene
        self.reset_scene(self._scene)

    def on_toggle_simulation_stepping(self):
        self.simulation_stepping_enabled = not self.simulation_stepping_enabled
        state_text = "running" if self.simulation_stepping_enabled else "paused"
        self._logger.info(f"Simulation is {state_text}")

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
                                                        on_reset=self.on_reset_message
                                                        )
        if _REGISTER_FRAME_LISTENER:
            self.app.addListener(self._click_frame_listener)

        self._keyboardListener = KeyboardListener(on_stop=self.on_stop,
                                                  on_reset=self.on_keyboard_reset,
                                                  on_toggle_stepping=self.on_toggle_simulation_stepping
                                                  )
        self.sim.add(self._keyboardListener)

    def enforce_step_realtime_settings(self):
        if self.app.getAutoStepping():
            self._logger.info(f"Something (SimulationView?) enabled autostepping, disabling it again...")
            self.app.setAutoStepping(False)
        if self.app.getRealTimeSync() is not self._realtime_sync:
            self._logger.info(f"Something (SimulationView?) changed realtime sync, forcing it to {self._realtime_sync} again...")
            self.app.setRealTimeSync(self._realtime_sync)
