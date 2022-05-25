#pragma once

#include <hook.h>

#include <memory>
#include <vector>

class _WrappedPlayer {
   private:
    Player *player;
    ServerNetworkHandler *handler;
    const NetworkIdentifier *id;

   public:
    _WrappedPlayer(const NetworkIdentifier *id, ServerNetworkHandler *handler, Player *player) : id(id), handler(handler), player(player) {}

    [[nodiscard]] bool isValid() { return player != nullptr; }

    std::string getName() { return isValid() ? player->getName() : ""; }
    std::string getXuid() { return isValid() ? player->getXuid() : ""; }

    void sendNetworkPacket(const Packet &p) const;
    inline void sendNetworkPacket(const Packet *p) const { sendNetworkPacket(*p); }
};

typedef std::shared_ptr<_WrappedPlayer> WrappedPlayer;