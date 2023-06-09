#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click
{
  namespace protobuf
  {
    class ControlMessage;
  }
  class ControlMessageBuilder;

  class ControlMessage : public Message
  {
  public:
    /**
     * Get angles, if any, for the object with name objectname
     *
     * \return a Vector of angles
     */
    CLICK_EXPORT std::vector<double> angles(const std::string &objectname) const;
    /**
     * Get anglevelocities, if any, for the object with name objectname
     *
     * \return a Vector of angle velocitities
     */
    CLICK_EXPORT std::vector<double> angleVelocities(const std::string &objectname) const;
    /**
     * Get torques, if any, for the object with name objectname
     *
     * \return a Vector of torques
     */
    CLICK_EXPORT std::vector<double> torques(const std::string &objectname) const;
    // TODO: Must implement hasControlEvent, will throw exception if missing!
    /**
     * Get controlEvent, if any, for the control controlname in object objectname
     *
     * \return value of controlEvent
     */
    CLICK_EXPORT bool controlEvent(const std::string &objectname, std::string controlname) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~ControlMessage();

  private:
    ControlMessage(std::unique_ptr<protobuf::ControlMessage> control_m);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ControlMessage> control_m;

    friend class ControlMessageBuilderImpl;
    friend class MessageSerializer;
  };
}
