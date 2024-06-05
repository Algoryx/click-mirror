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

  class CLICK_EXPORT ControlMessage : public Message
  {
  public:
    /**
     * @brief Check if object with objectname exists in control message
     *
     * @param objectname name of the object
     * @return true if the object exists
     * @return false if the object doesn't exist
     */
    bool has_object(const std::string& objectname);

    /**
     * Get values, if any, for the object with name objectname
     *
     * \return a Vector of angles
     */
    std::vector<double> values(const std::string& objectname) const;

    /**
     * Get angles, if any, for the object with name objectname
     *
     * \return a Vector of angles
     */
    std::vector<double> angles(const std::string& objectname) const;
    /**
     * Get angularvelocities, if any, for the object with name objectname
     *
     * \return a Vector of angle velocitities
     */
    std::vector<double> angularVelocities(const std::string& objectname) const;
    /**
     * Get torques, if any, for the object with name objectname
     *
     * \return a Vector of torques
     */
    std::vector<double> torques(const std::string& objectname) const;
    /**
     * Get controlEvent for the control controlname in object objectname
     *
     * \return value of controlEvent
     */
    bool controlEvent(const std::string& objectname, std::string controlname) const;
    /**
     * Check if controlEvent is present
     *
     * \return true if present, false if not.
     */
    bool hasControlEvent(const std::string& objectname, std::string controlname) const;
    MessageType messageType() const;
    std::string debugString() const;

    ~ControlMessage();

  private:
    ControlMessage(std::unique_ptr<protobuf::ControlMessage> control_m);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ControlMessage> control_m;

    friend class ControlMessageBuilderImpl;
    friend class MessageSerializer;
  };
}
