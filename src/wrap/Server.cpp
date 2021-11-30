//
// Created by lucas on 29/11/2021.
//

#include <iostream>
#include <wrap/TextPacket.h>
#include <wrap/Server.h>


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

void WrappedServer::broadcastPacket(Packet *packet) {
    handler->packetSender->sendBroadcast(packet);
}

void WrappedServer::handleEvent(const std::shared_ptr<HockEvent> &event) {
    HockEventType type = event->getType();
    switch (type) {
        case HockEventType::EVENT_MESSAGE: handleMessageEvent((MessageEvent *) event.get());break;
        case HockEventType::EVENT_PLAYER_LIST: handlePlayerList((PlayerListEvent *) event.get()); break;
        default:
            return;
    }
}

void WrappedServer::handleMessageEvent(const MessageEvent *event) {
    WrappedTextPacket packet(hook);

    switch ((MessageType) event->msgType) {
        case MessageType::NORMAL:
            if (event->translatable) {
                packet.createTranslatedChat(event->from, event->message);
            } else {
                packet.createChat(event->from, event->message);
            }
            break;
        case MessageType::ANOUNCE:
            if (event->translatable) {
                packet.createTranslatedAnnouncement(event->message);
            } else {
                packet.createAnnouncement(event->message);
            }
            break;
        case MessageType::WHISPER:
            packet.createWhisper(event->from, event->message);
            break;
        case MessageType::SYSTEM:
            packet.createSystemMessage(event->message);
            break;
        case MessageType::JUKEBOX:
            packet.createJukeboxPopup(event->message);
            break;
        default:
            std::cerr << "Unexpected message type: " << event->msgType << std::endl;
            return;
    }
    if (event->isDirected()) {
        if (!sendPacketTo(event->to, packet.get())) {
            std::cerr << "Cannot send message to player " << event->to << std::endl;
        }
        return;
    }

    broadcastPacket(packet.get());
}

void WrappedServer::handlePlayerList(const PlayerListEvent *) {
    std::cout << "Received request for player list" << std::endl;
    auto eventData = std::make_shared<PlayerListEvent>();
    eventData->players = hook->playerList();
    Hook::WriteOutputEvent(eventData);
}