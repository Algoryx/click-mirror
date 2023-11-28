import time


class WallClock:

    def __init__(self):
        self.time = self.now()

    def now(self):
        return time.time()

    def time_since_init(self):
        return self.now() - self.time

    def timestamp(self):
        return self.now()

    def time_since(self, timestamp: float) -> float:
        return self.now() - timestamp
