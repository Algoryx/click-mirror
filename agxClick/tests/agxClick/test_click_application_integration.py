from pClick.Messaging_pb2 import ControlMessage, ResetMessage, ResetMessageType, ValueType
import pytest
from pytest import approx
from subprocess import Popen, PIPE
from pClick import Client, MessageFactory, HandshakeMessageType, SensorMessageType, MessageSerializer
import zmq
from time import sleep
import platform


def send(client: Client, message):
    return client.socket.send(message.SerializeToString(), flags=zmq.NOBLOCK)


def send_receive(client: Client, message):
    send(client, message)

    tries = 10
    for i in range(tries):
        try:
            return MessageSerializer.from_bytes(client.socket.recv(flags=zmq.NOBLOCK))
        except zmq.Again as ex:
            sleep(1)
            print(f"(Waited {i} seconds)")
    raise Exception(f"Nothing to receive, even after {tries} tries")


@pytest.mark.integrationtest
class TestClickIntegration:
    """
    Base class for integration tests, does not contain any actual tests, only DSL-like methods used for testing in subclasses
    """

    @pytest.fixture(autouse=True)
    def cleanup(self):
        yield
        try:
            self.send_stop_simulation(self.client)
        except Exception as ex:
            pass
        self.process.kill
        stdout, stderr = self.process.communicate(timeout=1)
        print(stderr.decode("UTF-8"))
        print(stdout.decode("UTF-8"))

    def send_control_message(self, client):
        message = self.create_controlmessage()
        print("Sending control message")
        sensormessage = send_receive(client, message)
        assert sensormessage.messageType == SensorMessageType
        return sensormessage

    def send_sensor_request(self, client):
        message = MessageFactory.create_sensorrequestmessage()
        print("Sending sensor request message")
        sensormessage = send_receive(client, message)
        assert sensormessage.messageType == SensorMessageType
        return sensormessage

    def send_stop_simulation(self, client):
        message = MessageFactory.create_errormessage()
        print("Sending error request message")
        send(client, message)

    def send_handshake(self, client):
        message = MessageFactory.create_handshake_init()
        print("Sending handshake init message")
        handshake = send_receive(client, message)
        assert handshake.messageType == HandshakeMessageType
        return handshake

    def start_simulation(self, simulation_seconds, app_path, time_step, extra_flags="") -> Popen:
        python_executable = "agxpython" if platform.system() == "Linux" else "python3"
        cmd = f'{python_executable} examples/click_application.py --model testdata/ClickScene.yml:ExampleClickScene --timeStep {time_step} --stopAfter {simulation_seconds} --agxOnly {extra_flags}'
        print(f"Executing {cmd}")
        process = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True, cwd=app_path)
        return process

    def connect(self):
        client = Client()
        addr = "tcp://localhost:5555"
        print(f"Connecting to click server {addr}")
        client.connect(addr)
        return client

    def create_controlmessage(self):
        message = MessageFactory.create_controlmessage()
        robot = message.objects["robot1"]
        robot.angles.extend([0, 0])
        robot = message.objects["robot2"]
        robot.angles.extend([0, 0])
        return message


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
        assert sensormessage.simVars.simulatedTime == 0.0

        # The simulation has now started, but we must send another control message to verify since first simulatedTime is 0.0
        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime >= 0.1

        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime >= 0.2

        # Verify that handshake and sensor request can be sent after handshake as well
        handshake = self.send_handshake(client)

        sensormessage = self.send_sensor_request(client)
        assert sensormessage.simVars.simulatedTime > 0.0


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

    def start_simulation(self, simulation_seconds, app_path, time_step, extra_flags="") -> Popen:
        python_executable = "agxpython" if platform.system() == "Linux" else "python3"
        cmd = f'{python_executable} examples/click_application.py --model testdata/ClickScene.yml:ExampleDriveTrainClickScene --timeStep {time_step} --stopAfter {simulation_seconds} --agxOnly {extra_flags}'
        print(f"Executing {cmd}")
        process = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True, cwd=app_path)
        return process

    def test_that_controlmessage_has_effect(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()
        controlmessage = self.create_controlmessage()
        sensormessage = send_receive(client, controlmessage)
        assert sum(sensormessage.objects['robot'].angleSensors) != 0


@pytest.mark.integrationtest
class TestResetMessage(TestClickIntegration):
    def test_that_resetmessage_returns_resetmessage(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()

        sensormessage = self.send_control_message(client)
        assert sensormessage.simVars.simulatedTime == 0.0

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
