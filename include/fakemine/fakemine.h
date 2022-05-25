//
// Created by lucas on 28/11/2021.
// Fake structs to help use stuff
// These are created using bedrock server debug symbols
//

#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <bitset>

#include "base.h"
#include "TextPacket.h"
#include "serverPlayer.h"
#include "SynchedActorData.h"
#include "Dimension.h"
#include "ActorDamageSource.h"
#include "ExtendedCertificate.h"
#include "NetworkIdentifier.h"
#include "packets.h"
#include "binaryStream.h"
#include "ModalFormRequestPacket.h"
#include "ModalFormResponsePacket.h"

struct ServerNetworkHandler {
    void *unk0[8];
    struct GameCallbacks *gameCallbacks;
    void *nonOwnerPointer;
    void *unk1;
    struct NetworkHandler *networkHandler;
    struct PrivateKeyManager *privateKeyManager;
    struct ServerLocator *serverLocator;
    LoopbackPacketSender *packetSender;
    void *unk2;
    struct AllowList *allowList;
    struct PermissionsFile *permissionsFile;
    void *unk3[30];
    struct MinecraftCommands *minecraftCommands;
    struct Scheduler *scheduler;
    long unk4;
    struct ActiveTransfersManager *activeTransfersManager;
    struct ClassroomModeNetworkHandler *classRoomModeNetworkHandler;
    void *pad4[14];
    struct ServerNetworkController *serverNetworkController;
    void *pad5[42];

    ServerPlayer * _getServerPlayer(NetworkIdentifier const &, unsigned char);


};


class PacketSender {
public:
    virtual ~PacketSender() = 0;

    virtual void unknown() = 0;

    void sendToPrimaryClient(NetworkIdentifier const &, Packet const &);

    PacketSender(unsigned char);
};

class LoopbackPacketSender : public PacketSender {
public:
    virtual ~LoopbackPacketSender() = 0;

    virtual void unknown() = 0;

    virtual void send(Packet &) = 0;

    virtual void sendToServer(Packet &) = 0;

    virtual void sendToClient(NetworkIdentifier const *, Packet const *, unsigned char) = 0;

    virtual void sendToClients(std::vector<long> const &, Packet const &) = 0;

    virtual void sendBroadcast(Packet *) = 0;

    virtual void sendBroadcast(NetworkIdentifier const &, unsigned char, Packet const &) = 0;

    virtual void flush(NetworkIdentifier const &, std::function<void(void)> &&) = 0;

    LoopbackPacketSender(unsigned char, NetworkHandler &);

    void addLoopbackCallback(NetEventCallback &);

    void removeLoopbackCallback(NetEventCallback &);

    void setUserList(std::vector<std::unique_ptr<void *>> const *);
};

class TextObjectRoot;


struct ChangeDimensionRequest {
    int State;
    AutomaticID<Dimension, int> FromDimensionId;
    AutomaticID<Dimension, int> ToDimensionId;
    float posX;
    float posY;
    float posZ;
    bool UsePortal;
    bool Respawn;
    int AgentTag;
};

typedef int LogCategory;
typedef int LogRule;
// typedef int LogAreaID;


enum LogAreaID {
    Default,
};
class BedrockLog {
    public:
    enum LogCategory {
        DefaultL,
    };
    enum LogRule {
        DefaultR,
    };
    static void log(BedrockLog::LogCategory, std::bitset<3ul>, BedrockLog::LogRule, LogAreaID, unsigned int, char const*functionName, int functionLine, char const*format , ...);
};
