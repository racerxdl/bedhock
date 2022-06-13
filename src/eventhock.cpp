//
// Created by lucas on 29/11/2021.
//

#include <fmt/format.h>
#include <hook.h>
#include <json/serializer.h>

#include <iostream>
#include <thread>
#include <zmq.hpp>
#include <log.h>

void EventHockThread() {
    using namespace std::chrono_literals;
    Log::Info("EventHock Thread Started\n");
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::pair};
    socket.bind("tcp://*:5555");

    while (!Hook::IsStopped()) {
        bool rxEvent = false, txEvent = false;
        zmq::message_t request;
        auto res = socket.recv(request, zmq::recv_flags::dontwait);
        if (res > 0) {
            auto event = CJsonSerializer::DeserializeEvent(request.to_string());
            if (event != nullptr) {
                Hook::WriteInputEvent(event);
            }
            rxEvent = true;
        }

        auto e = std::make_shared<HockEvent>();
        if (Hook::ReadOutputEvent(e)) {
            std::string j;
            auto ok = CJsonSerializer::Serialize(e.get(), j);
            if (!ok) {
                Log::Error("Error serializing event\n");
            } else if (socket.handle() != nullptr) {  // Only send when connected
                auto res = socket.send(zmq::buffer(j), zmq::send_flags::dontwait);
                if (res.has_value() && res <= 0) {
                    Log::Error("Error sending event: code %d\n", res.value());
                    Hook::WriteOutputEvent(e); // Re-queue
                }
            }
            txEvent = true;
        }

        if (rxEvent || txEvent) {
            std::this_thread::yield();
        } else {
            std::this_thread::sleep_for(1ms);
        }
    }
    Log::Info("EventHock Thread Stopped\n");
}