#include <iostream>
#include <click/Client.h>
#include <click/ControlMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>

int main() {
    click::Client client;

    auto message = click::HandshakeInitMessageBuilder::builder()->build();
    std::cout << "Created message with contents: " << message->debugString() << std::endl;
}
