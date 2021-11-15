from pClick.Messaging_pb2 import ResetMessage, ResetMessageType
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
        cmd = f'env'
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
        robot = message.objects["panda_tool"]
        robot.angles.extend([0, 0, 0, 0, 0, 0, 0])
        robot = message.objects["panda_2"]
        robot.angles.extend([0, 0, 0, 0, 0, 0, 0])
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
class TestSensorRequest(TestClickIntegration):
    def test_that_sensorrequest_sends_values_not_zeros(self, pyroot):
        self.process = self.start_simulation(simulation_seconds=1.0, app_path=pyroot, time_step=0.1)
        self.client = client = self.connect()

        sensormessage = self.send_sensor_request(client)
        assert sensormessage.simVars.simulatedTime == 0.0
        assert sum(sensormessage.objects['panda_tool'].angleSensors) != 0


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
class TestResetMessageWithRCS(TestResetMessage):
    """
    Test that ResetMessage works ok with rcs as well, this tests the pushFrameToRemoteViewer path
    """

    def start_simulation(self, simulation_seconds, app_path, time_step, extra_flags=""):
        return TestResetMessage.start_simulation(self, simulation_seconds, app_path, time_step, extra_flags="--rcs")

    def test_that_resetmessage_returns_resetmessage(self, pyroot):
        TestResetMessage.test_that_resetmessage_returns_resetmessage(self, pyroot)
