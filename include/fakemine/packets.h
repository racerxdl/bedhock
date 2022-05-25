#pragma once

#include "base.h"

class MinecraftPackets {
   public:
    static std::shared_ptr<class Packet> createPacket(int type) {
        return createPacket((MinecraftPacketIds)type);
    }

    class MinecraftPackets &operator=(class MinecraftPackets const &) = delete;
    MinecraftPackets(class MinecraftPackets const &) = delete;
    MinecraftPackets() = delete;

   public:
    static class std::shared_ptr<class Packet> createPacket(enum MinecraftPacketIds);
};