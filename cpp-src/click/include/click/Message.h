#pragma once

#include <string>

namespace algoryx { namespace click {
  class Message
  {
  public:
    virtual std::string getDebugString() const = 0;
  };
}}
