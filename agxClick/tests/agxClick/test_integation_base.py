import pytest
from pytest import approx
from subprocess import Popen, PIPE
from pClick import Client, MessageFactory, HandshakeMessageType, SensorMessageType, MessageSerializer
import zmq
from time import sleep
import sys


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

    def start_simulation(self, simulation_seconds, app_path, time_step, model="testdata/ClickScene.yml:ExampleClickScene", extra_flags="") -> Popen:
        """
        If you provide a different model, you probably need to override create_controlmessage as well to match the chosen model
        """
        python_executable = sys.executable
        cmd = f'{python_executable} examples/click_application.py --model {model} --timeStep {time_step} --stopAfter {simulation_seconds} --agxOnly {extra_flags}'
        print(f"Executing {cmd}")
        process = Popen(cmd.split(" "), stdout=PIPE, stderr=PIPE, shell=False, cwd=app_path)
        return process

    def connect(self, addr: str = "tcp://localhost:5555"):
        client = Client()
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
