from agxClick import ClickBatchListener


class _BatchApplicationFake():
    def __init__(self, batch_time: float) -> None:
        self.time = 0
        self.num_resets = 0
        self.batch_listener = ClickBatchListener(None, batch_time, self.on_batch_end)

    def on_batch_end(self):
        self.num_resets += 1

    def step(self, time_step: float):
        self.batch_listener.preFrame(self.time)
        self.time += time_step


def equivalent(a: float, b: float, threshold: float = 0.00001):
    return abs(a - b) < threshold


class Test_batch_listener:
    def test_that_batch_is_reset(self):
        app = _BatchApplicationFake(1)
        num_steps = 21
        time_step = 0.1
        for i in range(num_steps):
            app.step(time_step)

        assert app.num_resets == 2
        assert equivalent(app.time - app.batch_listener.batch_start_time, 0.1)
