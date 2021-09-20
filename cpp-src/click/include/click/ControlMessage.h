#pragma once

#include <click/DllExport.h>
#include <click/Message.h>
#include <Messaging.pb.h>

#include <vector>
#include <memory>


namespace algoryx { namespace click {

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
    CLICK_EXPORT ControlMessage(protobuf::ControlMessage * control_m);
    CLICK_EXPORT virtual std::string serializeToBytes() const;

    protobuf::ControlMessage * control_m;

    friend class ControlMessageBuilder;
    friend class MessageSerializer;
  };
}}
