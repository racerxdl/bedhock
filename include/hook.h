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
    std::vector<subhook::Hook *> hooks;
    std::map<std::string, subhook::Hook *> nameToHook;
    std::map<ulong, NetworkIdentifier *> hashToNetworkIdentifier;
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

    subhook::Hook *getHook(const std::string &name);

    // Original hooked functions
    std::function<threeArgVoid> o_ServerNetworkHandler_displayGameMessage;
    std::function<threeArgVoid> o_Level_onPlayerDeath;
    std::function<threeArgVoid> o_ServerNetworkHandler_handle_PlayerAuthInputPacket;
    std::function<threeArgVoid> o_ServerNetworkHandler_onClientAuthenticated;
    std::function<threeArgVoid> o_ServerNetworkHandler_onPlayerLeft;
    std::function<oneArgVoid> o_ServerNetworkHandler_onTick;
    std::function<twoArgVoid> o_StartGamePacket_writeBinaryStream;

    // Event Queues
    SafeQueue<std::shared_ptr<HockEvent>> input, output;

protected:
    // Hook Callbacks
    static void *ServerNetworkHandler_displayGameMessage(void *thisObj, void *player, void *chatEvent);

    static void *Level_onPlayerDeath(void *thisObj, void *player, void *actorDamageSource);

    static void *ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, void *networkIdentifier, void *playerAuthInputPacket);

    static void *ServerNetworkHandler_onClientAuthenticated(void *thisObj, void *networkIdentifier, void *certificate);

    static void *ServerNetworkHandler_onPlayerLeft(void *thisObj, void *serverPlayer, void *something);

    static void *ServerNetworkHandler_onTick(void *thisObj);

    // Fixes
    static void *StartGamePacket_write_BinaryStream(void *thisObj, void *binaryStream);

    ~Hook();

public:
    static void InitSingleton(const SymbolMap &map);

    static void *getFunctionPointer(const std::string &name);

    NetworkIdentifier *getPlayerNetworkIdentifier(const std::string &name);

    // Bind only functions
    std::function<ulong(void *)> o_NetworkIdentifier_getHash;
    std::function<void(std::string &, void *)> o_ExtendedCertificate_getIdentityName;
    std::function<void(std::string &, void *)> o_ExtendedCertificate_getXuid;
    std::function<void(void *)> o_TextPacket_TextPacket;
    std::function<void(void *)> o_TextPacket_destructor;
    std::function<void(void *, std::string, std::string, std::string, std::string)> o_TextPacket_createChat;
    std::function<void(void *, std::string const &, std::string const &, std::string const &, std::string const &)> o_TextPacket_createTranslatedAnnouncement;
    std::function<void(void *, std::string)> o_TextPacket_createRaw;
    std::function<void(void *, std::string, std::vector<std::string>)> o_TextPacket_createTranslated;
    std::function<void(void *, std::string)> o_TextPacket_createSystemMessage;
    std::function<void(void *, std::string const &, std::string const &, std::string const &, std::string const &)> o_TextPacket_createAnnouncement;
    std::function<void(void *, std::string const &, std::string const &, std::string const &, std::string const &)> o_TextPacket_createTranslatedChat;
    std::function<void(void *, std::string const &, std::vector<std::string> const &)> o_TextPacket_createJukeboxPopup;
    std::function<void(void *, std::string const &, std::string const &, std::string const &, std::string const &)> o_TextPacket_createWhisper;

    std::vector<std::string> playerList();

    static void StopAll();
    static bool IsStopped() { return singleton->stopped; }

    // Event Queue Management
    static bool ReadInputEvent(std::shared_ptr<HockEvent> &event);

    static bool ReadOutputEvent(std::shared_ptr<HockEvent> &event);

    static void WriteInputEvent(std::shared_ptr<HockEvent> event);

    static void WriteOutputEvent(std::shared_ptr<HockEvent> event);
};