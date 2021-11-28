//
// Created by lucas on 28/11/2021.
//

#pragma once

#include <subhook.h>
#include <vector>
#include <functional>
#include "symbolmap.h"

typedef void *(oneArgVoid)(void *);
typedef void *(twoArgVoid)(void *, void *);
typedef void *(threeArgVoid)(void *, void *, void *);

class Hook {
private:
    static Hook *singleton;
    SymbolMap symMap;
    std::vector<subhook::Hook*> hooks;
    std::map<std::string, subhook::Hook*> nameToHook;
    Hook();
    void Init(const SymbolMap &sym);
    template <typename T> bool hookFunc(const std::string &name, const std::string &hname, std::function<T> &hookOrigFn, void *target);
    template <typename T> bool bind(const std::string &name, std::function<T> &hook);
    subhook::Hook* getHook(const std::string& name);

    // Original functions
    std::function<threeArgVoid> o_ServerNetworkHandler_displayGameMessage;
    std::function<threeArgVoid> o_Level_onPlayerDeath;
    std::function<threeArgVoid> o_ServerNetworkHandler_handle_PlayerAuthInputPacket;

    std::function<ulong(void*)> o_NetworkIdentifier_getHash;
protected:
    // Hook Callbacks
    static void *ServerNetworkHandler_displayGameMessage(void *thisObj, void *player, void *chatEvent);
    static void *Level_onPlayerDeath(void *thisObj, void *player, void *actorDamageSource);
    static void *ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, void *networkIdentifier, void *playerAuthInputPacket);

    ~Hook();
public:
    static void InitSingleton(const SymbolMap &map);
};