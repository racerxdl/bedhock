//
// Created by lucas on 29/11/2021.
//
#pragma once

#include <fakemine/fakemine.h>
#include <hook.h>

class WrappedServer {
    Hook *hook;
    ServerNetworkHandler *handler;
public:
    explicit WrappedServer(Hook *hook, void *serverNetworkHandler);
    void broadcastMessage(const std::string &sourceName, const std::string &message, bool needsTranslate = false);
    bool sendMessageTo(const std::string &sourceName, const std::string &destinationName, const std::string &message, bool needsTranslate = false);
    bool sendPacketTo(const std::string &destinationName, Packet *packet);
};
