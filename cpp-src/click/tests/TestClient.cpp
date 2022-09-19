#include <catch2/catch.hpp>
#include <click/Client.h>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace click;

class ClientSUT : public Client
{
public:
    ClientSUT() {}

    void destructorReplacement()
    {
        this->terminateAndWSAWarn();
    }

    virtual void terminate()
    {
        throw invalid_argument("WSASTARTUP");
    }
};

/*
 * NOTE: Because it is really hard to test the destructor, we test the only call made in the destructor instead.
 */
SCENARIO("Client usage")
{
    GIVEN("A Client")
    {
        std::unique_ptr<ClientSUT> client = make_unique<ClientSUT>();

        WHEN("The destructor detects an exception")
        {
            THEN("The exception is propagated")
            {
                REQUIRE_THROWS(client->destructorReplacement());
            }
        }
    }
}
