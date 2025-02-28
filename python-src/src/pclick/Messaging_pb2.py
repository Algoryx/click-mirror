# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: Messaging.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0fMessaging.proto\x12\x0e\x63lick.protobuf\"G\n\x07Message\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12\n\n\x02id\x18\x02 \x01(\x05\"r\n\x14HandshakeInitMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12(\n\x07version\x18\x02 \x01(\x0e\x32\x17.click.protobuf.Version\"H\n\x14SensorRequestMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\"\xff\x07\n\x10HandshakeMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12(\n\x07version\x18\x02 \x01(\x0e\x32\x17.click.protobuf.Version\x12.\n\x0b\x63ontrolType\x18\x03 \x01(\x0e\x32\x19.click.protobuf.ValueType\x12>\n\x07objects\x18\x04 \x03(\x0b\x32-.click.protobuf.HandshakeMessage.ObjectsEntry\x12H\n\x0bsimSettings\x18\x05 \x01(\x0b\x32\x33.click.protobuf.HandshakeMessage.SimulationSettings\x1a\x33\n\x07Sensors\x12(\n\x05types\x18\x01 \x03(\x0e\x32\x19.click.protobuf.ValueType\x1a\x9e\x04\n\x06Object\x12\x17\n\x0f\x63ontrolsInOrder\x18\x01 \x03(\t\x12\x36\n\x13\x63ontrolTypesInOrder\x18\x08 \x03(\x0e\x32\x19.click.protobuf.ValueType\x12\x1b\n\x13jointSensorsInOrder\x18\x07 \x03(\t\x12/\n\x0cjointSensors\x18\x02 \x03(\x0e\x32\x19.click.protobuf.ValueType\x12Q\n\rcontrolEvents\x18\x03 \x03(\x0b\x32:.click.protobuf.HandshakeMessage.Object.ControlEventsEntry\x12\x45\n\x07sensors\x18\x04 \x03(\x0b\x32\x34.click.protobuf.HandshakeMessage.Object.SensorsEntry\x12\x30\n\robjectSensors\x18\x06 \x03(\x0e\x32\x19.click.protobuf.ValueType\x1aO\n\x12\x43ontrolEventsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12(\n\x05value\x18\x02 \x01(\x0e\x32\x19.click.protobuf.ValueType:\x02\x38\x01\x1aX\n\x0cSensorsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x37\n\x05value\x18\x02 \x01(\x0b\x32(.click.protobuf.HandshakeMessage.Sensors:\x02\x38\x01\x1a&\n\x12SimulationSettings\x12\x10\n\x08timeStep\x18\x01 \x01(\x01\x1aW\n\x0cObjectsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x36\n\x05value\x18\x02 \x01(\x0b\x32\'.click.protobuf.HandshakeMessage.Object:\x02\x38\x01\"\xb5\x03\n\x0e\x43ontrolMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12<\n\x07objects\x18\x02 \x03(\x0b\x32+.click.protobuf.ControlMessage.ObjectsEntry\x1a\xdb\x01\n\x06Object\x12\x0e\n\x06\x61ngles\x18\x01 \x03(\x01\x12\x19\n\x11\x61ngularVelocities\x18\x02 \x03(\x01\x12\x0f\n\x07torques\x18\x03 \x03(\x01\x12\x0e\n\x06values\x18\x05 \x03(\x01\x12O\n\rcontrolEvents\x18\x04 \x03(\x0b\x32\x38.click.protobuf.ControlMessage.Object.ControlEventsEntry\x1a\x34\n\x12\x43ontrolEventsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x08:\x02\x38\x01\x1aU\n\x0cObjectsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x34\n\x05value\x18\x02 \x01(\x0b\x32%.click.protobuf.ControlMessage.Object:\x02\x38\x01\"\xdc\n\n\rSensorMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12;\n\x07objects\x18\x03 \x03(\x0b\x32*.click.protobuf.SensorMessage.ObjectsEntry\x12\x42\n\x07simVars\x18\x04 \x01(\x0b\x32\x31.click.protobuf.SensorMessage.SimulationVariables\x1a\x13\n\x04Vec3\x12\x0b\n\x03\x61rr\x18\x01 \x03(\x01\x1a\xd3\x04\n\x06Sensor\x12\x0f\n\x05\x61ngle\x18\x01 \x01(\x01H\x00\x12\x1b\n\x11\x61ngularVelocity1d\x18\x02 \x01(\x01H\x00\x12\x12\n\x08torque1d\x18\x03 \x01(\x01H\x00\x12\x36\n\x08position\x18\x04 \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x31\n\x03rpy\x18\x05 \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x13\n\tactivated\x18\x06 \x01(\x08H\x00\x12<\n\x0e\x61\x63\x63\x65leration3d\x18\x07 \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x35\n\x07\x66orce3d\x18\x08 \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x36\n\x08torque3d\x18\t \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x43\n\x15\x61ngularAcceleration3d\x18\n \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x38\n\nvelocity3d\x18\x0c \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12?\n\x11\x61ngularVelocity3d\x18\r \x01(\x0b\x32\".click.protobuf.SensorMessage.Vec3H\x00\x12\x11\n\x07\x66orce1d\x18\x0e \x01(\x01H\x00\x42\x07\n\x05value\x1a?\n\x07Sensors\x12\x34\n\x06sensor\x18\x01 \x03(\x0b\x32$.click.protobuf.SensorMessage.Sensor\x1a\xe7\x02\n\x06Object\x12\x14\n\x0c\x61ngleSensors\x18\x01 \x03(\x01\x12\x1e\n\x16\x61ngularVelocitySensors\x18\x02 \x03(\x01\x12\x15\n\rtorqueSensors\x18\x03 \x03(\x01\x12\x38\n\x0c\x66orceSensors\x18\x05 \x03(\x0b\x32\".click.protobuf.SensorMessage.Vec3\x12;\n\robjectSensors\x18\x04 \x03(\x0b\x32$.click.protobuf.SensorMessage.Sensor\x12\x42\n\x07sensors\x18\x06 \x03(\x0b\x32\x31.click.protobuf.SensorMessage.Object.SensorsEntry\x1aU\n\x0cSensorsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x34\n\x05value\x18\x02 \x01(\x0b\x32%.click.protobuf.SensorMessage.Sensors:\x02\x38\x01\x1a,\n\x13SimulationVariables\x12\x15\n\rsimulatedTime\x18\x01 \x01(\x01\x1aT\n\x0cObjectsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x33\n\x05value\x18\x02 \x01(\x0b\x32$.click.protobuf.SensorMessage.Object:\x02\x38\x01\"@\n\x0cResetMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\"\x80\x01\n\x0c\x45rrorMessage\x12\x30\n\x0bmessageType\x18\x01 \x01(\x0e\x32\x1b.click.protobuf.MessageType\x12(\n\x05\x65rror\x18\x02 \x01(\x0e\x32\x19.click.protobuf.ErrorType\x12\x14\n\x0c\x65rrormessage\x18\x03 \x01(\t*\xbe\x01\n\x0bMessageType\x12\x1c\n\x18HandshakeInitMessageType\x10\x00\x12\x18\n\x14HandshakeMessageType\x10\x01\x12\x1c\n\x18SensorRequestMessageType\x10\x06\x12\x16\n\x12\x43ontrolMessageType\x10\x02\x12\x15\n\x11SensorMessageType\x10\x03\x12\x14\n\x10ResetMessageType\x10\x04\x12\x14\n\x10\x45rrorMessageType\x10\x05*\xed\x01\n\tValueType\x12\t\n\x05\x41ngle\x10\x00\x12\x15\n\x11\x41ngularVelocity1D\x10\x01\x12\x0c\n\x08Torque1D\x10\x02\x12\x0c\n\x08Position\x10\x03\x12\x07\n\x03RPY\x10\x04\x12\r\n\tActivated\x10\x05\x12\x12\n\x0e\x41\x63\x63\x65leration3D\x10\x06\x12\x0b\n\x07\x46orce1D\x10\x0c\x12\x0b\n\x07\x46orce3D\x10\x07\x12\x0c\n\x08Torque3D\x10\x08\x12\x19\n\x15\x41ngularAcceleration3D\x10\t\x12\x0e\n\nVelocity3D\x10\n\x12\x15\n\x11\x41ngularVelocity3D\x10\x0b\x12\x0c\n\x08Multiple\x10\x0f*-\n\x07Version\x12\r\n\tUNDEFINED\x10\x00\x12\x13\n\x0f\x43URRENT_VERSION\x10\x01*$\n\tErrorType\x12\x17\n\x13VersionNotSupported\x10\x00\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'Messaging_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _HANDSHAKEMESSAGE_OBJECT_CONTROLEVENTSENTRY._options = None
  _HANDSHAKEMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_options = b'8\001'
  _HANDSHAKEMESSAGE_OBJECT_SENSORSENTRY._options = None
  _HANDSHAKEMESSAGE_OBJECT_SENSORSENTRY._serialized_options = b'8\001'
  _HANDSHAKEMESSAGE_OBJECTSENTRY._options = None
  _HANDSHAKEMESSAGE_OBJECTSENTRY._serialized_options = b'8\001'
  _CONTROLMESSAGE_OBJECT_CONTROLEVENTSENTRY._options = None
  _CONTROLMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_options = b'8\001'
  _CONTROLMESSAGE_OBJECTSENTRY._options = None
  _CONTROLMESSAGE_OBJECTSENTRY._serialized_options = b'8\001'
  _SENSORMESSAGE_OBJECT_SENSORSENTRY._options = None
  _SENSORMESSAGE_OBJECT_SENSORSENTRY._serialized_options = b'8\001'
  _SENSORMESSAGE_OBJECTSENTRY._options = None
  _SENSORMESSAGE_OBJECTSENTRY._serialized_options = b'8\001'
  _MESSAGETYPE._serialized_start=3337
  _MESSAGETYPE._serialized_end=3527
  _VALUETYPE._serialized_start=3530
  _VALUETYPE._serialized_end=3767
  _VERSION._serialized_start=3769
  _VERSION._serialized_end=3814
  _ERRORTYPE._serialized_start=3816
  _ERRORTYPE._serialized_end=3852
  _MESSAGE._serialized_start=35
  _MESSAGE._serialized_end=106
  _HANDSHAKEINITMESSAGE._serialized_start=108
  _HANDSHAKEINITMESSAGE._serialized_end=222
  _SENSORREQUESTMESSAGE._serialized_start=224
  _SENSORREQUESTMESSAGE._serialized_end=296
  _HANDSHAKEMESSAGE._serialized_start=299
  _HANDSHAKEMESSAGE._serialized_end=1322
  _HANDSHAKEMESSAGE_SENSORS._serialized_start=597
  _HANDSHAKEMESSAGE_SENSORS._serialized_end=648
  _HANDSHAKEMESSAGE_OBJECT._serialized_start=651
  _HANDSHAKEMESSAGE_OBJECT._serialized_end=1193
  _HANDSHAKEMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_start=1024
  _HANDSHAKEMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_end=1103
  _HANDSHAKEMESSAGE_OBJECT_SENSORSENTRY._serialized_start=1105
  _HANDSHAKEMESSAGE_OBJECT_SENSORSENTRY._serialized_end=1193
  _HANDSHAKEMESSAGE_SIMULATIONSETTINGS._serialized_start=1195
  _HANDSHAKEMESSAGE_SIMULATIONSETTINGS._serialized_end=1233
  _HANDSHAKEMESSAGE_OBJECTSENTRY._serialized_start=1235
  _HANDSHAKEMESSAGE_OBJECTSENTRY._serialized_end=1322
  _CONTROLMESSAGE._serialized_start=1325
  _CONTROLMESSAGE._serialized_end=1762
  _CONTROLMESSAGE_OBJECT._serialized_start=1456
  _CONTROLMESSAGE_OBJECT._serialized_end=1675
  _CONTROLMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_start=1623
  _CONTROLMESSAGE_OBJECT_CONTROLEVENTSENTRY._serialized_end=1675
  _CONTROLMESSAGE_OBJECTSENTRY._serialized_start=1677
  _CONTROLMESSAGE_OBJECTSENTRY._serialized_end=1762
  _SENSORMESSAGE._serialized_start=1765
  _SENSORMESSAGE._serialized_end=3137
  _SENSORMESSAGE_VEC3._serialized_start=1961
  _SENSORMESSAGE_VEC3._serialized_end=1980
  _SENSORMESSAGE_SENSOR._serialized_start=1983
  _SENSORMESSAGE_SENSOR._serialized_end=2578
  _SENSORMESSAGE_SENSORS._serialized_start=2580
  _SENSORMESSAGE_SENSORS._serialized_end=2643
  _SENSORMESSAGE_OBJECT._serialized_start=2646
  _SENSORMESSAGE_OBJECT._serialized_end=3005
  _SENSORMESSAGE_OBJECT_SENSORSENTRY._serialized_start=2920
  _SENSORMESSAGE_OBJECT_SENSORSENTRY._serialized_end=3005
  _SENSORMESSAGE_SIMULATIONVARIABLES._serialized_start=3007
  _SENSORMESSAGE_SIMULATIONVARIABLES._serialized_end=3051
  _SENSORMESSAGE_OBJECTSENTRY._serialized_start=3053
  _SENSORMESSAGE_OBJECTSENTRY._serialized_end=3137
  _RESETMESSAGE._serialized_start=3139
  _RESETMESSAGE._serialized_end=3203
  _ERRORMESSAGE._serialized_start=3206
  _ERRORMESSAGE._serialized_end=3334
# @@protoc_insertion_point(module_scope)
