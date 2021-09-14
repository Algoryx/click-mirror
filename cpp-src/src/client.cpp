#include <iostream>
#include "Messaging.pb.h"

using namespace std;

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    iam::ControlMessage * control_m = new iam::ControlMessage();
    control_m->set_messagetype(iam::ControlMessageType);
    google::protobuf::Map<string, iam::ControlMessage_Object> * map = control_m->mutable_objects();
    (*map)["robot1"] = iam::ControlMessage_Object();

    double src[] = { 1, 2, 3, 4, 5 };
    int n = sizeof(src) / sizeof(src[0]); 
    std::vector<double> data(src, src + n);
    (*map)["robot1"].mutable_anglevelocities()->Assign(data.begin(), data.end());
    (*(*map)["robot1"].mutable_controlevents())["gripper"] = true;
    cout << control_m->DebugString();
    cout << control_m->SerializeAsString().length();
    cout << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
