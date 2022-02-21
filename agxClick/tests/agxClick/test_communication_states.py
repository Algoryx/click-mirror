from agxClick.click_event_listener import States
import pytest


@pytest.mark.parametrize("state", [States.RECV_HANDSHAKE, States.RECV])
def test_that_recv_can_receive(state):
    assert state.can_receive()


@pytest.mark.parametrize("state", [States.READ_CONTROLS, States.INVALID, States.SEND_RESET, States.SEND_SENSORS])
def test_that_cannot_receive(state):
    assert not state.can_receive()


@pytest.mark.parametrize("state", [States.READ_CONTROLS, States.RECV_HANDSHAKE, States.RECV, States.SEND_RESET, States.SEND_SENSORS])
def test_that_valid(state):
    assert state.valid()


def test_that_invalid():
    state = States.INVALID
    assert not state.valid()


class ContainStatesClass:

    def __init__(self):
        self.state = States.READ_CONTROLS


class UseStatesClass:

    def __init__(self):
        self.statecontainer = ContainStatesClass()

    @property
    def state(self):
        return self.statecontainer.state

    @state.setter
    def state(self, new_state: States):
        self.statecontainer.state = new_state


def test_that_state_from_container_is_returned():
    userclass = UseStatesClass()

    assert userclass.state == States.READ_CONTROLS


def test_that_state_from_container_is_set():
    userclass = UseStatesClass()
    userclass.state = States.INVALID

    assert userclass.state == States.INVALID

