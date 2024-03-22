#pragma once
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <array>

namespace click
{
  namespace protobuf
  {
    class SensorMessage;
  }

  typedef std::array<double, 3> Vec3;

  union SensorValue
    {
      double angle;
      double angularVelocity;
      double torque;
      Vec3 position;
      Vec3 rpy;
      bool activated;
      Vec3 acceleration;
      Vec3 force;
      Vec3 directionalTorque;
      Vec3 angularAcceleration;
    };

  struct Sensor {
    ValueType type;
    SensorValue value;
  };

  class CLICK_EXPORT SensorMessage : public Message
  {
  public:
    /**
     * Get angles, if any, for the object with name objectname
     *
     * \return a Vector of angles
     */
    std::vector<double> angles(const std::string& objectname) const;
    /**
     * Get angular velocities, if any, for the object with name objectname
     *
     * \return a Vector of angle velocitities
     */
    std::vector<double> angularVelocities(const std::string& objectname) const;

    std::string debugString() const;
    MessageType messageType() const;

    /**
     * Get rotation for the object with name objectname
     *
     * \return a Vec3 with roll, pitch, yaw
     */
    Vec3 objectRPY(const std::string& objectname) const;
    /**
     * Get position for the object with name objectname
     *
     * \return a Vec3 with x, y, z
     */
    Vec3 objectPosition(const std::string& objectname) const;

    /**
     * Get sensor list for sensor with sensorname for object with name objectname
     *
     * \return a Vector of Sensors
     */
    std::vector<Sensor> sensor(const std::string& objectname, const std::string& sensorname) const;
    Vec3 sensorVec3(const std::string& objectname, const std::string& sensorname, int idx) const;
    double sensorDouble(const std::string& objectname, const std::string& sensorname, int idx) const;
    bool sensorBool(const std::string& objectname, const std::string& sensorname, int idx) const;
    double simulatedTime() const;

    /**
     * Get torques, if any, for the object with name objectname
     *
     * \return a Vector of torques
     */
    std::vector<double> torques(const std::string& objectname) const;

    ~SensorMessage();

  private:
    SensorMessage(std::unique_ptr<protobuf::SensorMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::SensorMessage> sensorMess;

    friend class SensorMessageBuilderImpl;
    friend class MessageSerializer;
  };

  std::unique_ptr<SensorMessage> toSensorMessage(std::unique_ptr<Message> message);
}
