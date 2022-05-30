//
// Created by lucas on 29/11/2021.
//

#include <fakemine/TextPacket.h>
#include <fmt/format.h>
#include <log.h>
#include <wrap/Server.h>
#include <wrap/TextPacket.h>

#include <iostream>

WrappedServer::WrappedServer(Hook *hook, void *serverNetworkHandler) {
    this->hook = hook;
    this->handler = (ServerNetworkHandler *)serverNetworkHandler;
}

void WrappedServer::broadcastMessage(const std::string &sourceName, const std::string &message, bool needsTranslate) {
    TextPacket packet =
        (needsTranslate) ? TextPacket::createTranslatedChat(sourceName, message) : TextPacket::createChat(sourceName, message);
    Log::Debug("Sending message {} to everyone\n", message);
    handler->packetSender->sendBroadcast(&packet);
}

bool WrappedServer::sendMessageTo(const std::string &sourceName, const std::string &destinationName, const std::string &message, bool needsTranslate) {
    TextPacket packet =
        (needsTranslate) ? TextPacket::createTranslatedChat(sourceName, message) : TextPacket::createChat(sourceName, message);
    Log::Debug("Sending message {} to {}\n", message, destinationName);
    return sendPacketTo(destinationName, &packet);
}

void WrappedServer::sendFormTo(const std::string &destinationName, unsigned int formId, const std::string &data) {
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::ModalFormRequest);
    auto mfr = (ModalFormRequestPacket *)pkt.get();
    mfr->formId = formId;
    mfr->jsonData = data;

    sendPacketTo(destinationName, pkt.get());
}

void WrappedServer::sendForm(unsigned int formId, const std::string &data) {
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::ModalFormRequest);
    auto mfr = (ModalFormRequestPacket *)pkt.get();
    mfr->formId = formId;
    mfr->jsonData = data;

    broadcastPacket(pkt.get());
}

void WrappedServer::sendTestTo(const std::string &destinationName) {
    auto identifier = hook->getPlayerNetworkIdentifier(destinationName);
    if (identifier == nullptr) {
        Log::Error("No such player {}\n", destinationName);
        return;
    }
    auto player = getServerPlayer(*identifier);
    if (player == nullptr) {
        Log::Error("No such server player {}\n", destinationName);
        return;
    }

    // std::cout << "SENT" << std::endl;
    // auto pkt = TextPacket::createSystemMessage("SENT FORM");
    // player->sendNetworkPacket(pkt);
}

bool WrappedServer::sendPacketTo(const std::string &destinationName, Packet *packet) {
    auto identifier = hook->getPlayerNetworkIdentifier(destinationName);
    if (identifier == nullptr) {
        return false;
    }
    if (packet == nullptr) {
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
        case HockEventType::EVENT_MESSAGE:
            handleMessageEvent((MessageEvent *)event.get());
            break;
        case HockEventType::EVENT_PLAYER_LIST:
            handlePlayerList((PlayerListEvent *)event.get());
            break;
        case HockEventType::EVENT_FORM_REQUEST:
            handleFormRequest((FormRequestEvent *)event.get());
            break;
        case HockEventType::EVENT_SCOREBOARD:
            handleScoreboard((ScoreBoardEvent *)event.get());
            break;
        default:
            fmt::print("Received unknown event {}\n", (int)type);
            return;
    }
}

void WrappedServer::handleFormRequest(const FormRequestEvent *event) {
    if (event->isDirected()) {
        sendFormTo(event->to, event->formId, event->jsonData);
        return;
    }

    sendForm(event->formId, event->jsonData);
}

void WrappedServer::handleScoreboard(const ScoreBoardEvent *event) {
    auto displayPacket = SetDisplayObjectivePacket(event->type, event->name, event->title, "dummy", event->sortDescending ? ObjectiveSortOrder::Descending : ObjectiveSortOrder::Ascending);
    std::vector<ScorePacketInfo> info;
    for (const auto &p: event->entries) {
        auto k = ScorePacketInfo(
            ScoreboardId(p.entryId),
            event->name,
            IdentityDefinition::Type::Fake,
            p.score,
            p.entryName
        );
        k.pid = p.pid;
        k.aid = p.actorId;
        info.push_back(k);
    }
    auto scorePacket = SetScorePacket(info);

    if (event->isDirected()) {
        sendPacketTo(event->to, &displayPacket);
        sendPacketTo(event->to, &scorePacket);
        return;
    }

    broadcastPacket(&displayPacket);
    broadcastPacket(&scorePacket);
}

void WrappedServer::handleMessageEvent(const MessageEvent *event) {
    TextPacket packet = TextPacket::createFromEvent(event);
    if (event->isDirected()) {
        if (!sendPacketTo(event->to, &packet)) {
            Log::Error("Cannot send message to player {}\n", event->to);
        }
        return;
    }

    broadcastPacket(&packet);
}

void WrappedServer::handlePlayerList(const PlayerListEvent *) {
    auto eventData = std::make_shared<PlayerListEvent>();
    eventData->players = hook->playerList();
    Hook::WriteOutputEvent(eventData);
}