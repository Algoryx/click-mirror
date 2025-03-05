"""
Click Integration Tests
"""
import pytest
from pytest import approx
from click_integration import TestClickIntegration, send_receive, demoserver_cmd # pylint: disable=W0611,unused-import
from pclick import HandshakeMessageType, SensorMessageType

# pylint: disable=attribute-defined-outside-init # pytest Test classes must not have an __init__()
# pylint: disable=redefined-outer-name,arguments-differ # allow using pytest fixtures


@pytest.mark.using_pclick
@pytest.mark.integrationtest
class TestHappyPath(TestClickIntegration):

    def test_that_demoserver_returns_different_messages(self):
        self.process = self.start_simulation()
        self.client = client = self.connect()

        handshake = self.send_handshake(client)
        assert handshake.messageType == HandshakeMessageType

        sensormessage = self.send_control_message(client)
        assert sensormessage.messageType == SensorMessageType

