//
// Created by lucas on 29/11/2021.
//
#pragma once

#include <event/form.h>
#include <event/message.h>
#include <event/playerlist.h>
#include <event/scoreboard.h>
#include <event/playerDeath.h>
#include <fakemine/fakemine.h>
#include <hook.h>

class WrappedServer {
   private:
    Hook *hook;
    ServerNetworkHandler *handler;
    void handleMessageEvent(const MessageEvent *event);
    void handlePlayerList(const PlayerListEvent *event);
    void handleFormRequest(const FormRequestEvent *event);
    void handleScoreboard(const ScoreBoardEvent *event);
    void handlePlayerDeathRequest(const PlayerDeathCountRequestEvent *event);

   public:
    inline WrappedPlayer getServerPlayer(NetworkIdentifier const &id) {
        return std::make_shared<_WrappedPlayer>(&id, handler, (Player *)handler->_getServerPlayer(id, 0));
    }
    explicit WrappedServer(Hook *hook, void *serverNetworkHandler);
    void handleEvent(const std::shared_ptr<HockEvent> &event);

    void broadcastMessage(const std::string &sourceName, const std::string &message, bool needsTranslate = false);
    void broadcastPacket(Packet *packet);
    bool sendMessageTo(const std::string &sourceName, const std::string &destinationName, const std::string &message, bool needsTranslate = false);
    bool sendPacketTo(const std::string &destinationName, Packet *packet);
    void sendFormTo(const std::string &destinationName, unsigned int formId, const std::string &data);
    void sendForm(unsigned int formId, const std::string &data);
    void sendTestTo(const std::string &destinationName);
};
