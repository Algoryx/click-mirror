#pragma once
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace algoryx { namespace click {
  class SensorMessage : public Message
  {
  public:
    CLICK_EXPORT SensorMessage();

    CLICK_EXPORT std::string getDebugString() const;
    CLICK_EXPORT ~SensorMessage();

  private:
    std::vector<double> m_jointAngles;
  };
}}
