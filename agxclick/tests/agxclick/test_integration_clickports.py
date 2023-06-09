import pytest
from pytest import approx
from test_integation_base import TestClickIntegration


@pytest.mark.integrationtest
class TestDifferentConnectionPort(TestClickIntegration):
    def test_that_different_ports_can_be_used(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, extra_flags=f"--addr tcp://*:8888")
        self.client = client = self.connect("tcp://localhost:8888")

        handshake = self.send_handshake(client)
        assert handshake.simSettings.timeStep == approx(0.1)

    def test_that_ipc_can_be_used(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, extra_flags=f"--addr ipc:///tmp/click.ipc")
        self.client = client = self.connect("ipc:///tmp/click.ipc")

        handshake = self.send_handshake(client)
        assert handshake.simSettings.timeStep == approx(0.1)
