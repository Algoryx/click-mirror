import signal
import sys
from agxclick import KeyboardListener, AgxApplication, ClickFrameListener, ApplicationStepListener, ResetBatchListener
from pclick import Server
from pclick.server import SizeCollector, SizeCollectorChanges
from typing import Any, Callable, List
import logging
from agxclick.wallclock import WallClock
from agxclick.graphics_throttler import GraphicsThrottler


class ClickApplication(AgxApplication):

    def signal_handler(self, sig, frame):
        self._logger.info("Stopping from SIGINT")
        self.on_stop()

    def __init__(self, args: List[str]):
        super().__init__(args)
        self._logger = logging.getLogger(__file__)
        self._stop_application = False
        self.args = self.parse_arguments(args)
        self.simulation_stepping_enabled = self.args.start_paused is False
        self.disable_clicksync = self.args.disable_clicksync is True
        self._click_frame_listener = None
        self._reset_batch_listener = None
        self.application_step_listeners: List[ApplicationStepListener] = []
        self.server = None
        signal.signal(signal.SIGINT, self.signal_handler)

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
        # Since we drive the graphics from here, disable ExampleApplication graphics throttling
        self.app.setTargetFPS(0)

        self._logger.info(f"Entering main loop {'with' if self.app.getViewer() else 'without'} graphics targeting {self.args.framerate} Hz")
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
        # Release license seat - important if docker/floating license.
        self.stop()

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
        gfx_throttler = GraphicsThrottler(self.args.framerate)
        click_sync_enabled = not self.disable_clicksync
        while not self._stop_application:
            # Reset wall_clock when handshake completed if syncing simulation step with controlmessage
            if click_sync_enabled and not self._click_frame_listener.handshake_completed():
                wall_clock = WallClock()
                num_frames = 0

            for listener in self.application_step_listeners:
                listener.preFrame(self.sim.getClock().getTime())
            click_ready_for_simstep = self.disable_clicksync or self._click_frame_listener.can_step_simulation()
            if click_ready_for_simstep and self.simulation_stepping_enabled:
                self.stepSimulation()
            if (gfx_throttler.render_graphics(wall_clock)):
                self.stepApplication()
                self.enforce_step_realtime_settings()
                num_frames += 1
            if self.args.stopAfterFrame and self.sim.getClock().getFrame() >= self.args.stopAfterFrame:
                self._stop_application = True
            if self.args.stopAfter and self.sim.getClock().getTime() >= self.args.stopAfter:
                self._stop_application = True
            if self.args.trace_sizes and self.server.size_collector.is_updated:
                print(f"Received {self.server.size_collector.recv_size} from client, Sent {self.server.size_collector.send_size} bytes to client")

        return num_frames, wall_clock

    def report_timing(self, num_frames, wall_clock):
        simsteps = self.sim.getClock().getFrame()
        walltime = wall_clock.time_since_init()
        self._logger.info(f"Rendered {num_frames} frames and {simsteps} simulation steps, received {self._click_frame_listener.num_controls_received} control messages")
        if (simsteps != self._click_frame_listener.num_controls_received):
            missed_simsteps = simsteps - self._click_frame_listener.num_controls_received
            self._logger.warn(f"Controller could not keep up with simulation! {missed_simsteps} simulation steps taken without control message")
        self._logger.info(f"simulated time: {self.sim.getClock().getTime()} Wall clock time: {walltime}")
        self._logger.info(f"Wallclock sim freq: {simsteps/walltime:.1f} Hz Wallclock framerate: {num_frames/walltime:.1f}")
        return simsteps, walltime

    def parse_arguments(self, args):
        from argparse import ArgumentParser
        parser = ArgumentParser(args)
        parser.add_argument('--addr', metavar='<addr>', type=str, default="tcp://*:5555",
                            help=f'set server address. Ie ipc:///tmp/click.ipc, default is tcp://*:5555')
        parser.add_argument('--batch', type=float, default=None,
                            help="Enable automatic restart of the scene after the specified number of seconds, with updated values for Brick BatchVariables and ParameterSpace variables")
        parser.add_argument('--disableClickSync', dest='disable_clicksync', action="store_true",
                            help="Do not sync each simulation step with click client - simulation will run without waiting for control messages")
        parser.add_argument('--framerate', type=int, default=60,
                            help="Specify target framerate in fps. 0 means match simulation framerate. default is 60. Recommended is 30-60. Only affects agxViewer, typically no speedup in browser")
        parser.add_argument('--profile', dest='profile', action="store_true", help="CProfile main loop and print results")
        parser.add_argument('--profileFile', type=str, default="", help="Write profile data to binary file (for snakeviz) instead of stdout")
        parser.add_argument('--startPaused', dest='start_paused', action="store_true", help="Start with simulation paused")
        parser.add_argument('--stopAfter', type=float, default=None, help="Stop when this simulation time is reached")
        parser.add_argument('--stopAfterFrame', type=int, default=None, help="Stop when this number of simulation steps has been executed")
        parser.add_argument('--trace-sizes', action='store_true',
                            help=f'print size of what is sent/received')
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

    def reset_scene(self, scene_to_reset):
        if (self._reset_batch_listener is not None):
            self._reset_batch_listener.prepare_for_next_batch_state()

        super().reset_scene(scene_to_reset)

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
        for listener in self.application_step_listeners:
            listener.update_scene(scene)

    def add_listeners(self):
        self._logger.info(f"Starting server on {self.args.addr}")
        size_collector = SizeCollectorChanges() if self.args.trace_sizes else SizeCollector()
        self.server = Server(self.args.addr, size_collector)
        self._click_frame_listener = ClickFrameListener(scene=None, app=self.app,
                                                        on_stop=self.on_stop,
                                                        on_exception=self.on_exception,
                                                        on_reset=self.on_reset_message,
                                                        server=self.server
                                                        )
        self.application_step_listeners.append(self._click_frame_listener)
        if self.args.batch is not None:
            self._reset_batch_listener = ResetBatchListener(scene=None, batch_time=self.args.batch,
                                                            on_batch_end=self.on_keyboard_reset)
            self.application_step_listeners.append(self._reset_batch_listener)
        keyboardListener = KeyboardListener(on_stop=self.on_stop,
                                            on_reset=self.on_keyboard_reset,
                                            on_toggle_stepping=self.on_toggle_simulation_stepping
                                            )
        self.sim.add(keyboardListener)

    def enforce_step_realtime_settings(self):
        if self.app.getAutoStepping():
            self._logger.info(f"Something (SimulationView?) enabled autostepping, disabling it again...")
            self.app.setAutoStepping(False)
        if self.app.getRealTimeSync() is not self._realtime_sync:
            self._logger.info(f"Something (SimulationView?) changed realtime sync, forcing it to {self._realtime_sync} again...")
            self.app.setRealTimeSync(self._realtime_sync)
