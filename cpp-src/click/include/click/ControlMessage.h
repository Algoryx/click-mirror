#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace algoryx { namespace click {
  class ControlMessage : public Message
  {
  public:
    CLICK_EXPORT ControlMessage();

    CLICK_EXPORT std::vector<double> getJointTorques() const;

    CLICK_EXPORT void addJointTorque(double torque);
    CLICK_EXPORT void setJointTorques(std::vector<double>& torques);

    CLICK_EXPORT std::string getDebugString() const;

    CLICK_EXPORT ~ControlMessage();

  private:
    bool m_triggerStep;
    std::vector<double> m_jointTorques;
  };
}}
