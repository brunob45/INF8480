#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "operation.grpc.pb.h"

#define TRACEPOINT_DEFINE

#include "lttng-traces/grpc_tracing.h"


using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::Status;
using grpc::StatusCode;
using operation::Operation;
using operation::OperationRequest;
using operation::OperationReply;

class OperationManager {
public:
    OperationManager(std::shared_ptr <Channel> channel)
            : stub_(Operation::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back from the server
    std::string ProcessOperation(const std::string &task) {
        char cstr1[task.size() + 1];
        strcpy(cstr1, task.c_str());
        tracepoint(grpc_tracing, manager_send, cstr1);//DO NOT MODIFY
        OperationRequest request;
        request.set_task(task);// Data we are sending to the server.
        OperationReply reply; // Container for the data we expect from the server.

        // Context for the client. It could be used to convey extra information to the server and/or tweak certain RPC behaviors.
        ClientContext context;

//TODO : send the gRPC call
        Status status(StatusCode::OK, "test"); // dummy status response

        char cstr2[reply.message().size() + 1];
        strcpy(cstr2, reply.message().c_str());
        tracepoint(grpc_tracing, manager_recv, cstr2);//DO NOT MODIFY
        
        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        }

        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        return status.error_message();
    }

private:
    std::unique_ptr <Operation::Stub> stub_;
};

int main(int argc, char **argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs  are created.
    //This channel models a connection to an endpoint
    ChannelArguments args;
    // Set the load balancing policy for the channel.
    args.SetLoadBalancingPolicyName("round_robin");

    OperationManager manager(grpc::CreateCustomChannel(
            "localhost:50051", grpc::InsecureChannelCredentials(), args));

//TODO : modify this code to take the task as command line parameter
    if (argc < 2)
    {
        std::cout << "mettre le fichier en paramètre" << std::endl;
    }

    std::string task(argv[1]); // std::string task("tache envoyee");
    std::string reply = manager.ProcessOperation(task);
    std::cout << "Reply from server : " << reply << std::endl;

    return 0;
}
