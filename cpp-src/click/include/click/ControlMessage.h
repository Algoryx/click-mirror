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
//    CLICK_EXPORT std::vector<double> getJointTorques() const;

    CLICK_EXPORT std::vector<double>angles(std::string objectname) const;
    CLICK_EXPORT std::vector<double>angleVelocities(std::string objectname) const;
    CLICK_EXPORT std::vector<double>torques(std::string objectname) const;
    CLICK_EXPORT bool controlEvent(std::string objectname, std::string controlname) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string DebugString() const;

    CLICK_EXPORT ~ControlMessage();

  private:
    ControlMessage(protobuf::ControlMessage * control_m);

    protobuf::ControlMessage * control_m;

    friend class ControlMessageBuilder;
  };
}}
