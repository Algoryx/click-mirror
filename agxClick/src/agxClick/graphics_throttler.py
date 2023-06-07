from agxclick.wallclock import WallClock

class GraphicsThrottler:

    def __init__(self, framerate: int):
        """
        If framerate is 0 or negative -> render_graphics always returns True
        """
        self.framerate = framerate
        self.last_render_timestamp = 0

    def render_graphics(self, wall_clock: WallClock, auto_reset=True) -> bool:
        """
        """
        if self.framerate <= 0:
            return True
        render = wall_clock.time_since(self.last_render_timestamp) >= 1.0 / self.framerate
        if render and auto_reset:
            self.rendered(wall_clock)
        return render

    def rendered(self, wall_clock: WallClock):
        self.last_render_timestamp = wall_clock.timestamp()
