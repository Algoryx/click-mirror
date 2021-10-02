#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click { namespace protobuf {
  class ControlMessage;
}}

namespace click {

  class ControlMessageBuilder;


  class ControlMessage : public Message
  {
  public:
    CLICK_EXPORT std::vector<double> angles(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> angleVelocities(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> torques(const std::string &objectname) const;
    CLICK_EXPORT bool controlEvent(const std::string &objectname, std::string controlname) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~ControlMessage();

  private:
    CLICK_EXPORT ControlMessage(std::unique_ptr<protobuf::ControlMessage> control_m);
    CLICK_EXPORT virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ControlMessage> control_m;

    friend class ControlMessageBuilderImpl;
    friend class MessageSerializer;
  };
}
