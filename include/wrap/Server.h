//
// Created by lucas on 29/11/2021.
//
#pragma once

#include <fakemine/fakemine.h>
#include <hook.h>
#include <event/message.h>
#include <event/playerlist.h>

class WrappedServer {
    Hook *hook;
    ServerNetworkHandler *handler;
public:
    explicit WrappedServer(Hook *hook, void *serverNetworkHandler);
    void broadcastMessage(const std::string &sourceName, const std::string &message, bool needsTranslate = false);
    void broadcastPacket(Packet *packet);
    bool sendMessageTo(const std::string &sourceName, const std::string &destinationName, const std::string &message, bool needsTranslate = false);
    bool sendPacketTo(const std::string &destinationName, Packet *packet);
    void handleEvent(const std::shared_ptr<HockEvent>& event);
    void handleMessageEvent(const MessageEvent *event);
    void handlePlayerList(const PlayerListEvent *event);
};
