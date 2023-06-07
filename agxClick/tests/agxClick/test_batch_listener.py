from pytest import approx, fixture
from agxclick import ResetBatchListener


class _BatchApplicationFake():
    def __init__(self, batch_time: float) -> None:
        self.time = 0
        self.num_resets = 0
        self.batch_listener = ResetBatchListener(None, batch_time, self.on_batch_end)

    def on_batch_end(self):
        self.batch_listener.prepare_for_next_batch_state()
        self.num_resets += 1

    def step(self, time_step: float):
        self.batch_listener.preFrame(self.time)
        self.time += time_step
    
    def reset(self):
        self.on_batch_end()
        


class Test_batch_listener:

    @fixture(autouse=True)
    def _setup(self):
        self.time_step = 0.1
        self.end_time = 1.0
        self.app = _BatchApplicationFake(self.end_time)

    def step_sim(self, n):
        for _ in range(n):
            self.app.step(self.time_step)

    def test_that_batch_runs_specified_last_timestep(self):
        self.step_sim(10)

        assert self.app.num_resets == 0
        assert self.app.time == approx(self.end_time)

    def test_that_batch_is_reset(self):
        self.step_sim(11)

        assert self.app.num_resets == 1
        assert self.app.time == approx(self.end_time + self.time_step)

    def test_that_batch_is_reset_twice(self):
        self.step_sim(21)

        assert self.app.num_resets == 2
        assert self.app.time - self.app.batch_listener.batch_start_time == approx(self.time_step)

    def test_batch_start_time(self):
        self.step_sim(11)

        assert self.app.batch_listener.batch_start_time == approx(10.0 * self.time_step)

    def test_batch_reset_command_will_reset(self):
        self.step_sim(4)
        self.app.reset()

        assert self.app.num_resets == 1
        assert self.app.batch_listener.batch_start_time == approx(3.0 * self.time_step)
    
    def test_batch_reset_command_will_restart_batch_time(self):
        self.step_sim(4)
        self.app.reset()    
        self.step_sim(9)

        assert self.app.num_resets == 1
        
        self.step_sim(1)
        
        assert self.app.num_resets == 2
