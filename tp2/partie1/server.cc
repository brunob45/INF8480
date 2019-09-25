/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "operation.grpc.pb.h"

#define TRACEPOINT_DEFINE
#include "lttng-traces/grpc_tracing.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using operation::Operation;
using operation::OperationRequest;
using operation::OperationReply;

// Logic and data behind the server's behavior
class ServiceImpl final : public Operation::Service {
    Status ProcessOperation(ServerContext *context, const OperationRequest *request,
                            OperationReply *reply) override {
        int id = 50051;
        tracepoint(grpc_tracing, server_start, id);
        reply->set_message(request->task());
        tracepoint(grpc_tracing, server_end, id);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with clients
    builder.RegisterService(&service);
    // Finally assemble the server
    std::unique_ptr <Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    // Wait for the server to shutdown
    server->Wait();
}

int main(int argc, char **argv) {
    RunServer();

    return 0;
}
