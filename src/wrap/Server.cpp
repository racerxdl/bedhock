//
// Created by lucas on 29/11/2021.
//

#include <wrap/TextPacket.h>
#include "wrap/Server.h"


WrappedServer::WrappedServer(Hook *hook, void *serverNetworkHandler) {
    this->hook = hook;
    this->handler = (ServerNetworkHandler *) serverNetworkHandler;
}

void WrappedServer::broadcastMessage(const std::string &sourceName, const std::string &message, bool needsTranslate) {
    WrappedTextPacket packet(hook);
    if (needsTranslate) {
        packet.createTranslatedChat(sourceName, message);
    } else {
        packet.createChat(sourceName, message);
    }
    handler->packetSender->sendBroadcast(packet.get());
}

bool WrappedServer::sendMessageTo(const std::string &sourceName, const std::string &destinationName, const std::string &message, bool needsTranslate) {
    WrappedTextPacket packet(hook);
    if (needsTranslate) {
        packet.createTranslatedChat(sourceName, message);
    } else {
        packet.createChat(sourceName, message);
    }

    return sendPacketTo(destinationName, packet.get());
}

bool WrappedServer::sendPacketTo(const std::string &destinationName, Packet *packet) {
    auto identifier = hook->getPlayerNetworkIdentifier(destinationName);
    if (identifier == nullptr) {
        return false;
    }

    handler->packetSender->sendToClient(identifier, packet, 0);
    return true;
}