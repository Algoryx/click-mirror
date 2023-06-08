from pclick.Messaging_pb2 import ResetMessageType
import pytest
from pytest import approx
from subprocess import Popen
from pclick import MessageFactory
from test_integation_base import TestClickIntegration, send_receive


@pytest.mark.integrationtest
class TestHappyPath(TestClickIntegration):
    def test_that_simulation_starts_after_handshake(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()

        handshake = self.send_handshake(client)
        assert handshake.simSettings.timeStep == approx(0.1)

        sensormessage = self.send_sensor_request(client)
        assert sensormessage.simVars.simulatedTime == 0.0

        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime == pytest.approx(0.1)

        # The simulation has now started, but we must send another control message to verify since first simulatedTime is 0.0
        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime == pytest.approx(0.2)

        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime == pytest.approx(0.3)

        # Verify that handshake and sensor request can be sent after handshake as well
        handshake = self.send_handshake(client)

        sensormessage = self.send_sensor_request(client)
        assert sensormessage.simVars.simulatedTime == pytest.approx(0.3)


@pytest.mark.integrationtest
class TestHappyPathWithoutClickSync(TestClickIntegration):
    def test_that_simulation_does_not_wait_for_handshake(self, pyroot):
        self.process: Popen = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, extra_flags="--disableClickSync")

        # If we send a message, we might wait indefinitely if process already finished, therefore resort to stderr parsing
        stdout, stderr = self.process.communicate(None, timeout=10)
        assert "simulated time: 1.0" in stderr.decode("UTF-8")


@pytest.mark.integrationtest
class TestSensorRequest(TestClickIntegration):
    def test_that_sensorrequest_sends_values_not_zeros(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()

        sensormessage = self.send_sensor_request(client)
        assert sensormessage.simVars.simulatedTime == 0.0
        assert sum(sensormessage.objects['robot1'].angleSensors) != 0


@pytest.mark.integrationtest
class TestVelocityControlMessage(TestClickIntegration):
    def create_controlmessage(self):
        message = MessageFactory.create_controlmessage()
        robot = message.objects["robot"]
        robot.angleVelocities.extend([1, 1])
        print(robot)
        return message

    def test_that_controlmessage_triggers_simulation_step(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, model="testdata/ClickScene.yml:ExampleDriveTrainClickScene")
        self.client = client = self.connect()
        controlmessage = self.create_controlmessage()
        sensormessage = send_receive(client, controlmessage)
        assert sum(sensormessage.objects['robot'].angleSensors) != 0.0


@pytest.mark.integrationtest
class TestResetMessage(TestClickIntegration):
    def test_that_resetmessage_returns_resetmessage(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()

        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime == pytest.approx(0.1)

        message = MessageFactory.create_resetmessage()
        print("Sending reset message")
        sensormessage = send_receive(client, message)
        assert sensormessage.messageType == ResetMessageType


@pytest.mark.integrationtest
class TestBatch(TestClickIntegration):
    def test_that_batch_sends_resetmessage(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, extra_flags="--batch 0.2")
        self.client = client = self.connect()

        # tick three steps for sim time to reach batch limit 0.2
        message = send_receive(client, self.create_controlmessage())
        message = send_receive(client, self.create_controlmessage())
        message = send_receive(client, self.create_controlmessage())

        assert message.messageType == ResetMessageType


@pytest.mark.integrationtest
class TestBatchReset(TestClickIntegration):
    def test_that_batch_resets_due_to_a_resetmessage(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1, extra_flags="--batch 0.2")
        self.client = client = self.connect()

        # tick one step
        message = send_receive(client, self.create_controlmessage())

        message = MessageFactory.create_resetmessage()
        # send reset message
        message = send_receive(client, message)

        # three steps after the reset message is sent we get a new reset
        message = send_receive(client, self.create_controlmessage())
        message = send_receive(client, self.create_controlmessage())
        message = send_receive(client, self.create_controlmessage())

        assert message.messageType == ResetMessageType


@pytest.mark.integrationtest
class TestResetMessageWithRCS(TestResetMessage):
    """
    Test that ResetMessage works ok with rcs as well, this tests the pushFrameToRemoteViewer path
    """

    def start_simulation(self, simulation_seconds, app_path, time_step, extra_flags=""):
        return TestResetMessage.start_simulation(self, simulation_seconds, app_path, time_step, extra_flags="--rcs")

    def test_that_resetmessage_returns_resetmessage(self, pyroot):
        TestResetMessage.test_that_resetmessage_returns_resetmessage(self, pyroot)
