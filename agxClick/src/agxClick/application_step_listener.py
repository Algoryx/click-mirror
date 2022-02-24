class ApplicationStepListener():
    def __init__(self, scene=None) -> None:
        self._scene = scene

    def preFrame(self, time: float):
        pass

    def update_scene(self, scene):
        self._scene = scene


def noop(*args, **kwargs: None):
    pass
