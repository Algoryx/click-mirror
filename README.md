# Click

## Why Click?

If you are using Brick to express your Robot Physics in Yaml, Click adds the low latency communication you need to let your controller control your robots in the simulation like they were real robots. Click automatically finds your robots in a Brick scene and allows controller environments to send and receive signals without forcing your controller to depend on Brick. If you want receive updates on additional objects, you add a Click configuration to Brick, pointing to which objects should be communicated.

Click implements C++ and Python clients. Additional language support can be added using protobuf code generation tooling.

## Click parts

There are currently three main parts of click

- [agxClick](agxClick/README.md) - a Simulation application using pClick, AGX and agxBrick that implements Click out of the box for a Brick model containing Robot(s).
- click - C++ click library with a democlient.
- pClick - Python click library with a demo client and demo server

The Click library implements creating, sending, receiving and interpreting messages across multiple platforms. Click can be used without agxClick, but the real benefit of Brick integration comes with agxClick.

## Introduction

The main idea behind click is to enable a non-Brick controller talking to a Brick enabled AGX Simulation in way configurable by Brick.
The name comes from the sound two Bricks makes when connected.

There are three main considerations

1. How the controller can send controls and receive sensor values in a similar fashion regardless of environment, ie real or sim.
2. How Brick adds sensors or topology on top of urdf, and how this is communicated to the controller.
3. How to communicate controls and sensors in an effective way.

The current solution is to introduce a Handshake, which enables the simulation to tell the controller what to expect in terms of how to control and what sensor data is being sent.

![Messaging overview diagram](doc/messaging.drawio.svg)

## Messaging Flow

Messaging is peer to peer, where client and server take turn. Ie

- Client loops over send/recv
- Server loops over recv/send.

The messages are

Request              | Response         | Comment
---------------------|------------------|--------
HandshakeInitMessage | HandshakeMessage |
SensorRequestMessage | SensorMessage    |
ControlMessage       | SensorMessage    | Client must also be prepared for ErrorMessage and ResetMessage
ResetMessage         | ResetMessage     | Sending a ResetMessage will pause the simulation
ErrorMessage         | -                | Sending an ErrorMessage will end the simulation

### Typical flow

A typical flow is

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. [Optional step] Client sends SensorRequestMessage to get initial values
5. [Optional step] Server responds with SensorMessage without starting simulation
6. Client sends ControlMessage, **this message will start the simulation**
7. Server responds with SensorMessage
8. The loop 6-8 is repeated.

### ResetMessage

When the Client sends a ResetMessage, the Server

- will respond with ResetMessage
- will reset the simulation to the intial state
- will pause the simulation
- will not start the simulation until the first received ControlMessage, the client can send a SensorRequestMessage without starting simulation at this point.

The typical flow therefore is

1. Client sends ResetMessage
2. Server responds with ResetMessage
3. [Optional] Client sends SensorRequestMessage
4. [Optional] Server responds with SensorMessage without starting simulation
5. Client sends ControlMessage
6. Server sends SensorMessage

The same will happen when the Simulation is reset from simulation side, eg a user reset the simulation:

1. Server responds with ResetMessage on any incoming Message (except ErrorMessage)
2. [Optional] Client sends SensorRequestMessage
3. [Optional] Server responds with SensorMessage without starting simulation
4. Client sends ControlMessage
5. Server sends SensorMessage

### HandshakeMessage

The HandshakeMessage contains both

- A Deprecated ControlType per scene
- The new ControlType per joint

Using ControlType per joint is preferred, but ControlType per scene is still supported but only **when all joint input signals are the same type**
The server can always send ValueType.Multiple as ControlType, meaning client must check the Control Type of every joint.
The server may (for backward compatibility) send ValueType.Angle, ValueType.AngleVelocity or ValueType.Torque if all joints have that same Control Type.

Also see ControlMessage below!

### ControlMessage

The ControlMessage contains mutually exclusive

- values - meaning each value may be of different type as specified in handshake
- angles (Deprecated) - meaning all values are Angle values as specified in handshake
- angleVelocities (Deprecated) - meaning all values are AngleVelocitiy values as specified in handshake
- torques (Deprecated) - meaning all values are Torque values as specified in handshake

### Stepping the simulation

The simulation is not started until after the first handshake is complete, ie when the first ControlMessage is received.
After that, the simulation is stepped once per message, except after a ResetMessage; see above.

## Installing

- Python, all platforms: Go to [agxClick](agxClick/README.md#install) for python install instructions. If you only want click and not agxClick, then do:

```bash
pip install pClick --extra-index-url https://click-access:rCsE-NdRsaknXcceBPVM@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
```

- Linux Ubuntu 20.04 C++ libraries and binaries:

```bash
wget --header "DEPLOY-TOKEN: rCsE-NdRsaknXcceBPVM" -O /tmp/click-shared-focal-amd64.deb "https://git.algoryx.se/api/v4/projects/algoryx%2Fexternal%2Fclick/packages/generic/click/0.1.34/click-shared-focal-amd64.deb"
apt-get install -yf /tmp/click-shared-focal-amd64.deb
```

- Windows C++ libraries and binaries
  - [Download win32 zip](https://click-access:rCsE-NdRsaknXcceBPVM@git.algoryx.se/api/v4/projects/262/packages/generic/click/0.1.34/click-win32-0.1.34.zip)
  - [Download x64 zip](https://click-access:rCsE-NdRsaknXcceBPVM@git.algoryx.se/api/v4/projects/262/packages/generic/click/0.1.34/click-x64-0.1.34.zip)

## Running Click democlient and demoserver

**Go to [agxClick](agxClick/README.md#Usage%20Examples) for brick model examples.**  
After installing (or building from source as specified below), run these commands in separate prompts:

```bash
python3 -m pClick.demo.server
```

```bash
build/bin/democlient
```

## Developing Click

This section and below are for developers developing Click

### Development Links

- [C++ ControlMessage example](cpp-src/click/tests/test_control_message.cpp)
- [C++ democlient](cpp-src/democlient/src/democlient.cpp)
- [Python demoserver](python-src/src/server.py)
- [Current protobuf schema](protobuf-src/Messaging.proto)
- [Technology choices etc](doc/messaging.md)

### Directory Structure with CMake relevant files expanded

```text
├──cpp-src
|  ├──click
|  |  ├──include/click
|  |  ├──shared_conf
|  |  ├──src
|  |  ├──tests
|  |  |  └──CMakeLists.txt
|  |  └──CMakeLists.txt
|  ├──CMakeModules
|  ├──democlient
|  |  ├──src
|  |  └──CMakeLists.txt
|  └──CMakeLists.txt
├──doc
├──docker
├──protobuf-src
|  └──CMakeLists.txt
├──python-src
|  ├──src
|  |  └──pClick
|  └──tests
|     └──pClick
└──testdata
```

### Build, test, and install click c++ library from source

NOTE: -DCMAKE_INSTALL_PREFIX=install makes install in build/install.

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR=Ninja -DCMAKE_INSTALL_PREFIX=install ../cpp-src
ninja && ninja test install
```

or for shared library, add BUILD_SHARED_LIBS:

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR=Ninja -DCMAKE_INSTALL_PREFIX=install -DBUILD_SHARED_LIBS=ON ../cpp-src
```

`ninja click-tests && ninja test` will compile test dependencies and run tests in one step.

### Reset c++ builds

```bash
cd build
rm -rf * .github.conan.cmake
```

### Build and test python pClick from source

NOTE: Read [agxClick](agxClick/README.md) to build and test agxClick

```bash
# Install requirements
pip3 install -r python-src/requirements-test.txt
# Install click locally
pip3 install -e python-src
# Run tests
pytest python-src
# Run demo server
python3 -m pClick.demo.server
```

The generated protobuf python code is committed in git repo.
It is created as part of c++ build, but can be created by running protoc as below.

```bash
protoc -I=protobuf-src --python_out=python-src/src Messaging.proto
```

### Build pip archive

```bash
pip3 install -r python-src/requirements-publish.txt
python3 setup.py sdist bdist_wheel
```

### Test frameworks

#### C++ catch2 framework

We are using [Catch2](https://github.com/catchorg/Catch2/) testing framework with cmake integration

- [catch2 matchers](https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top)

#### Python pytest

Python tests are using pytest

#### Python remarks

To circumvent clashes with python library click and pyClick, the python library is called pClick.

### Release:s

Releases are made by pushing the tag to be released, GitLab will build, test and publish the version.
Release tags are semver only, eg 0.1.2.

Steps:

1. Update version in agxClick/setup.py, python-src/setup.py, README.md. Suggestion: Search and Replace old version for new.
2. Update releaselog.txt
3. Push to branch and do MR
4. When MR merged, [create a tag in web IDE](https://git.algoryx.se/algoryx/external/click/-/tags/new) or tag from main and push, eg `git tag 0.1.16; git push origin 0.1.16`.
5. When built, the new release is available in the [Package Registry](https://git.algoryx.se/algoryx/external/click/-/packages)

## Build pipeline dependencies - updating agxBrick version

agxClick tests uses the generic Docker Image `registry.algoryx.se/algoryx/external/agx-docker/agxbrick-minified:latest`.
Therefore when raising minimum required agxBrick version, [agxBrick needs to be updated in the upstream](https://git.algoryx.se/algoryx/external/agx-docker) as well in order for tests to run.
