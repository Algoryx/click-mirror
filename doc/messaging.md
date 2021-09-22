# Click messaging

## Example messages

See [Current protobuf schema](../protobuf-src/Messaging.proto) for overview examples, and c++ and python tests for detailed examples, ie [C++ ControlMessage example](../cpp-src/click/tests/test_control_message.cpp) and [../python-src/tests/test_messaging.py].

### Message structure

- Message
  - ControlMessage - from client only
  - SensorMessage  - from sensor only
  - ResetMessage - from either
  - HandshakeInitMessage - from client only
  - HandshakeMessage - from server only
  - ErrorMessage - from server if bad version

### Significant Classes

- Message
  - HandshakeInitMessage
  - HandshakeMessage
  - ControlMessage
  - SensorMessage
  - ResetMessage
  - ErrorMessage
  ...
- MessageSerializer: parses Message --> Bytes, and Bytes --> Message, ie parse(bytes):Message, serialize(message): Bytes
- *MessageBuilder - create Complex Messages
- MessageFactory - create Simple Messages

## Technolocy and Design Choices

- 0MQ is an obvious choice to start with because of
  - it's simplicity
  - no middleware needed
  - L-GPL licence with link exception
  - Support for IPC communication
- Handshake vs configuration file
  - Using brick on controller side
    - Adds unwanted effort and binary size
    - Impose using brick instead of urdf
  - Starting with a handshake as opposed to a configuration file.
    - Reading another configuration file which also needs to be shared is more complex.
- Protobuf vs flatbuffers
  - Not a strong preference, choosing to start with protobuf mainly because of
    - adoption
    - simplicity.
- Specify units in protocol.
  - Not to start with, specified outside for now.
