import agxSDK
from typing import Callable
from agxPythonModules.utils.environment import application


class KEYS:
    n = 110


def noop(*args, **kwargs: None):
    pass


class KeyboardListener(agxSDK.GuiEventListener):

    def __init__(self,
                 on_stop: Callable[[], None] = noop,
                 on_reset: Callable[[], None] = noop):
        """
        on_stop: callback for when Esc is pressed
        on_reset: callback for when n is pressed
        """
        super().__init__(agxSDK.GuiEventListener.KEYBOARD)
        self._on_stop = on_stop
        self._on_reset = on_reset

    def keyboard(self, key, modKey, x, y, keyDown: bool):

        if not keyDown:
            return False

        if key == agxSDK.GuiEventListener.KEY_Escape:
            self._on_stop()
            return True

        if key == KEYS.n:
            self._on_reset()
            return True

        return False
