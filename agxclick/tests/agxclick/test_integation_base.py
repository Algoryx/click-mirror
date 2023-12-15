from time import sleep
import sys
from subprocess import Popen, PIPE
import pytest
import zmq
from pclick import Client, MessageFactory, HandshakeMessageType, SensorMessageType, MessageSerializer


def send(client: Client, message):
    return client.socket.send(message.SerializeToString(), flags=zmq.NOBLOCK)


def send_receive(client: Client, message):
    send(client, message)

    tries = 1000
    for i in range(tries):
        try:
            res = MessageSerializer.from_bytes(client.socket.recv(flags=zmq.NOBLOCK))
            print(f"(Waited {i/100} seconds)")
            return res
        except zmq.Again:
            sleep(0.01)
            # if (i % 100) == 0:
            #     print(f"(Waited {i/100} seconds)")
    raise TimeoutError(f"Nothing to receive, even after {tries} tries")


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
        except Exception:
            pass
        try:
            print("Waiting for brickview to exit")
            stdout, stderr = self.process.communicate(timeout=10)
            print(f"brickview stdout/stderr:")
            print(stderr.decode("UTF-8"))
            print(stdout.decode("UTF-8"))
        finally:
            try:
                # Make sure the process is killed so that next test is not blocked
                self.process.kill()
            finally:
                try:
                    # Really, make sure the process is killed so that next test is not blocked
                    os.kill(self.process.pid, 9)
                except Exception as ex:
                    pass

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
