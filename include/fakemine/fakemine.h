//
// Created by lucas on 28/11/2021.
// Fake structs to help use stuff
// These are created using bedrock server debug symbols
//

#pragma once

#include <memory>

class LoopbackPacketSender;

struct ServerNetworkHandler {
    void **onPlayerReadyCallback;
    void *unk0[5];
    struct GameCallbacks *gameCallbacks;
    long **unk1;
    long unk2;
    struct NetworkHandler *networkHandler;
    struct PrivateKeyManager *privateKeyManager;
    struct ServerLocator *serverLocator;
    LoopbackPacketSender *packetSender;
    char pad0[8];
    struct AllowList *allowList;
    struct PermissionsFile *permissionsFile;
    char pad1[4][16];
    bool unk3;
    char pad2[183];
    struct MinecraftCommands *minecraftCommands;
    struct Scheduler *scheduler;
    long unk4;
    struct ActiveTransfersManager *activeTransfersManager;
    struct ClassroomModeNetworkHandler *classRoomModeNetworkHandler;
    void *pad4[14];
    struct ServerNetworkController *serverNetworkController;
    void *pad5[42];
};

struct ServerNetworkController {};
struct GameCallbacks {};
struct Scheduler {};
struct PrivateKeyManager {};
struct MinecraftCommands {};
struct ActiveTransfersManager {};
struct ClassroomModeNetworkHandler {};
struct BinaryStream {};
struct ReadOnlyBinaryStream {};
struct NetworkIdentifier {};
struct NetEventCallback {};

#pragma pack(push, 4)

struct Packet {
    // vtable added by compiler
    int unk_4 = 2, unk_8 = 1;
    char pad[0x8];
    uintptr_t **handlerThunk;
    char pad2[0x4];

    virtual ~Packet() = 0;

    virtual int getId() const = 0;

    virtual std::string getName() const = 0;

    virtual void write(BinaryStream &) const = 0;

    virtual void read(ReadOnlyBinaryStream &) = 0;

    virtual void handle(NetworkIdentifier const &, NetEventCallback &, std::shared_ptr<Packet> &) = 0;
};

#pragma pack(pop)

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

    virtual void sendToClient(NetworkIdentifier const &, Packet const &, unsigned char) = 0;

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

class TextPacket;
