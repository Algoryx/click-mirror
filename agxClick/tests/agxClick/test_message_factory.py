import math
from clicktools import MessageFactory


class _SignalFake:

    def __init__(self, val):
        self._val = val

    def GetData(self):
        return self._val


class Test_message_factory:

    def test_that_converting_signals_to_radians_returns_radians(self):
        signals = [_SignalFake(180.0), _SignalFake(90)]
        assert MessageFactory._degs_to_radians(signals) == [math.pi, math.pi / 2]
