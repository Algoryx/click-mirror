from agxClick.graphics_throttler import GraphicsThrottler
from agxClick.wallclock import WallClock


def test_graphics_throttler_defaults_to_always_zero():
    throttler = GraphicsThrottler(0)

    assert throttler.render_graphics(None)


def test_graphics_throttler_always_renders_first_call():
    throttler = GraphicsThrottler(10)
    wallclock = WallClock()
    wallclock.now = lambda: wallclock.time + wallclock.time
    assert throttler.render_graphics(wallclock)


def test_graphics_throttler_waits():
    throttler = GraphicsThrottler(10)
    wallclock = WallClock()
    wallclock.now = lambda: wallclock.time
    assert throttler.render_graphics(wallclock)

    wallclock.now = lambda: 0.09 + wallclock.time
    assert throttler.render_graphics(wallclock) is False

    wallclock.now = lambda: 0.1001 + wallclock.time
    assert throttler.render_graphics(wallclock) is True
