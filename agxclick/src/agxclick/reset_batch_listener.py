from agxclick import ApplicationStepListener, noop
from typing import Callable


class ResetBatchListener(ApplicationStepListener):
    def __init__(self, scene, batch_time: float, on_batch_end: Callable[[], None] = noop) -> None:
        super().__init__()
        self._scene = scene
        self._batch_time = batch_time
        self.batch_start_time = 0.0
        self.current_time = 0.0
        self._on_batch_end = on_batch_end

    def geq(self, a: float, b: float, threshold: float = 0.00001):
        return a > b or (a - b) > -threshold

    def batch_has_ended(self, time):
        return self.geq(time, self.batch_start_time + self._batch_time)

    def prepare_for_next_batch_state(self):
        self.update_parameters()
        self.restart_batch_time(self.current_time)

    def preFrame(self, time: float):
        self.current_time = time
        if self.batch_has_ended(time):
            self.prepare_for_next_batch_state()
            self._on_batch_end()

    def update_parameters(self):
        if self._scene is not None:
            self._scene.UpdateBatchConfiguration()
            self._scene.UpdateParameterSpace()

    def restart_batch_time(self, time: float):
        self.batch_start_time = time
