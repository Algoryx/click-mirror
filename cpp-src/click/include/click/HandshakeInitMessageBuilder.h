#include<string>
#include<memory>
#include <click/HandshakeInitMessage.h>

class algoryx::click::HandshakeInitMessageBuilder {

public:
    std::unique_ptr<HandshakeInitMessage> build();
    static std::unique_ptr<HandshakeInitMessageBuilder> builder();

private:
    HandshakeInitMessageBuilder(std::unique_ptr<protobuf::HandshakeInitMessage> control_m);
    std::unique_ptr<protobuf::HandshakeInitMessage> message;
};
