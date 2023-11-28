from agxclick.wallclock import WallClock


def test_wallclock():
    wallclock = WallClock()
    wallclock.now = lambda: 1 + wallclock.time

    assert wallclock.time_since_init() == 1
    assert wallclock.timestamp() == wallclock.now()
    the_past = wallclock.now() - 5
    assert wallclock.time_since(the_past) == 5
