"""
Base class and some helper methods for Click integration tests
"""
import sys
import os
import platform
from pathlib import Path
from subprocess import Popen, PIPE
from time import sleep
import pytest
from pclick import Client, MessageFactory, HandshakeMessage, HandshakeMessageType, SensorMessageType, MessageSerializer
from zmq import Again, NOBLOCK # pylint: disable=no-name-in-module # pylint does not understand zmq imports

# pylint: disable=redefined-outer-name,attribute-defined-outside-init # allow using pytest fixtures


def send(client: Client, message):
    return client.socket.send(message.SerializeToString(), flags=NOBLOCK)


def send_receive(client: Client, message):
    send(client, message)

    tries = 1000
    for i in range(tries):
        try:
            res = MessageSerializer.from_bytes(client.socket.recv(flags=NOBLOCK))
            print(f"(Waited {i/100} seconds)")
            return res
        except Again:
            sleep(0.01)
    raise TimeoutError(f"Nothing to receive, even after {tries} tries")

@pytest.fixture
def demoserver_cmd():
    return "../oos/bin/demoserver.exe" if platform.system() == "Windows" else "../oos/bin/demoserver"

@pytest.mark.integrationtest
class TestClickIntegration:
    """
    Base class for integration tests, does not contain any actual tests, only DSL-like methods used for testing in subclasses
    """

    # Below init are just to get rid of pylint warnings
    client = None
    process = None

    @pytest.fixture(autouse=True)
    def setup(self, demoserver_cmd):
        self.sut: str = demoserver_cmd
        self.port: str = "5555"

    @pytest.fixture(autouse=True)
    def cleanup(self):
        yield
        try:
            self.send_stop_simulation(self.client)
            self.client.stop()
        # pylint: disable=duplicate-code
        finally:
            try:
                stdout, stderr = self.process.communicate(timeout=1)
                print("openplx_view stdout/stderr:")
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
                    except Exception as _ex: # pylint: disable=broad-except # We want to catch all exceptions here
                        pass
        # pylint: enable:duplicate-code

    def send_control_message(self, client):
        message = self.create_controlmessage()
        print("Sending control message")
        sensormessage = send_receive(client, message)
        assert sensormessage.messageType == SensorMessageType
        return sensormessage

    def send_stop_simulation(self, client):
        message = MessageFactory.create_errormessage()
        print("Sending error request message")
        send(client, message)

    def send_handshake(self, client) -> HandshakeMessage:
        message = MessageFactory.create_handshake_init()
        print("Sending handshake init message")
        handshake = send_receive(client, message)
        assert handshake.messageType == HandshakeMessageType
        return handshake

    def start_simulation(self) -> Popen:
        cmd = f'{self.sut} --trace'
        print(f"Executing {cmd}")
        process = Popen(cmd.split(" "), stdout=PIPE, stderr=PIPE, shell=False) # pylint: disable=consider-using-with # done in cleanup()
        return process

    def connect(self):
        client = Client()
        addr = f"tcp://localhost:{self.port}"
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
