//
// Created by lucas on 29/11/2021.
//

#include <iostream>
#include <hook.h>
#include <json/serializer.h>
#include <thread>
#include <zmq.hpp>

void EventHockThread() {
    using namespace std::chrono_literals;
    std::cout << "EventHock Thread Started" << std::endl;
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::pair};
    socket.bind("tcp://*:5555");

    while(!Hook::IsStopped()) {
        zmq::message_t request;
        auto res = socket.recv(request, zmq::recv_flags::dontwait);
        if (res > 0) {
            auto event = CJsonSerializer::DeserializeEvent(request.to_string());
            if (event != nullptr) {
                Hook::WriteInputEvent(event);
            }
        }

        auto e = std::make_shared<HockEvent>();
        if (Hook::ReadOutputEvent(e)) {
            std::string j;
            auto ok = CJsonSerializer::Serialize(e.get(), j);
            if (!ok) {
                std::cerr << "Error serializing event" << std::endl;
            } else if (socket.handle() != nullptr) { // Only send when connected
                socket.send(zmq::buffer(j), zmq::send_flags::dontwait);
            }
        }

        std::this_thread::sleep_for(10ms);
    }
    std::cout << "EventHock Thread Stopped" << std::endl;
}