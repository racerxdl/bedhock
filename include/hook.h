//
// Created by lucas on 28/11/2021.
//

#pragma once

#include <subhook.h>
#include <vector>
#include <functional>
#include <fakemine/fakemine.h>
#include <event/event.h>
#include <atomic>
#include <fmt/format.h>
#include <fmt/color.h>

#include "wrap/Player.h"
#include "symbolmap.h"
#include "SafeQueue.hpp"

#define MAX_QUEUE_LENGTH 512

typedef void *(oneArgVoid)(void *);

typedef void *(twoArgVoid)(void *, void *);

typedef void *(threeArgVoid)(void *, void *, void *);

class Hook {
private:
    static Hook *singleton;
    SymbolMap symMap;
    std::vector<std::shared_ptr<subhook::Hook>> hooks;
    std::map<std::string, std::shared_ptr<subhook::Hook>> nameToHook;
    std::map<ulong, const NetworkIdentifier *> hashToNetworkIdentifier;
    std::map<std::string, ulong> playerToHash;
    std::map<ulong, std::string> hashToPlayer;
    std::map<std::string, std::string> playerToXuid;

    std::atomic<bool> stopped{};

    Hook();

    void Init(const SymbolMap &sym);

    template<typename T>
    bool hookFunc(const std::string &name, const std::string &hname, std::function<T> &hookOrigFn, void *target);

    template<typename T>
    bool bind(const std::string &name, std::function<T> &hook);

    std::shared_ptr<subhook::Hook> getHook(const std::string &name);

    // Original hooked functions
    std::function<threeArgVoid> o_ServerNetworkHandler_displayGameMessage;
    std::function<threeArgVoid> o_ServerNetworkHandler_handle_ModalFormResponsePacket;
    std::function<threeArgVoid> o_Level_onPlayerDeath;
    std::function<threeArgVoid> o_ServerNetworkHandler_handle_PlayerAuthInputPacket;
    std::function<threeArgVoid> o_ServerNetworkHandler_onClientAuthenticated;
    std::function<threeArgVoid> o_ServerNetworkHandler_onPlayerLeft;
    std::function<oneArgVoid> o_ServerNetworkHandler_onTick;
    std::function<twoArgVoid> o_StartGamePacket_writeBinaryStream;
    std::function<threeArgVoid> o_Level_playerChangeDimension;

    // Event Queues
    SafeQueue<std::shared_ptr<HockEvent>> input, output;

protected:
    // Hook Callbacks
    static void *ServerNetworkHandler_displayGameMessage(void *thisObj, void *player, void *chatEvent);

    static void *Level_onPlayerDeath(void *thisObj, Player *player, ActorDamageSource *actorDamageSource);

    static void *ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, NetworkIdentifier *networkIdentifier, void *playerAuthInputPacket);

    static void *ServerNetworkHandler_handle_ModalFormResponsePacket(void *thisObj, NetworkIdentifier *networkIdentifier, ModalFormResponsePacket *modalFormResponsePacket);

    static void *ServerNetworkHandler_onClientAuthenticated(void *thisObj, const NetworkIdentifier &networkIdentifier, const Certificate &certificate);

    static void *ServerNetworkHandler_onPlayerLeft(void *thisObj, ServerPlayer *serverPlayer, void *something);

    static void *ServerNetworkHandler_onTick(void *thisObj);

    static void *Level_playerChangeDimension(void *thisObj,  Player *player, ChangeDimensionRequest *changeDimensionRequest);

    static void *bedLog(LogCategory category, int bitset,LogRule rules,LogAreaID area, unsigned int unk0, char *functionName,int functionLine, char *format,...);

    // Fixes
    static void *StartGamePacket_write_BinaryStream(void *thisObj, void *binaryStream);

    ~Hook();

public:

    static void InitSingleton(const SymbolMap &map);

    static void *getFunctionPointer(const std::string &name);

    const NetworkIdentifier *getPlayerNetworkIdentifier(const std::string &name);

    std::vector<std::string> playerList();

    static void StopAll();
    static bool IsStopped() { return singleton->stopped; }

    // Event Queue Management
    static bool ReadInputEvent(std::shared_ptr<HockEvent> &event);

    static bool ReadOutputEvent(std::shared_ptr<HockEvent> &event);

    static void WriteInputEvent(std::shared_ptr<HockEvent> event);

    static void WriteOutputEvent(std::shared_ptr<HockEvent> event);

};