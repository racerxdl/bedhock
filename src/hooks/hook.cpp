//
// Created by lucas on 28/11/2021.
//

#include <symbolmap.h>
#include "hook.h"
#include "hookNames.h"

#define TYPE_NAME(x) #x

#include <iostream>

#define HOOKFUNC(type, symbol, source, target) (hookFunc<type>(symbol, #target, source, (void *)target))
#define BIND(type, symbol, target) (bind<type>(symbol, target))

Hook *Hook::singleton;

Hook::Hook() = default;

void Hook::InitSingleton(const SymbolMap &map) {
    std::cout << "Initializing hooks" << std::endl;
    singleton = new Hook();
    singleton->Init(map);
}

Hook::~Hook() {
    for (auto h: this->hooks) {
        delete (h);
    }
}

void Hook::Init(const SymbolMap &sym) {
    symMap = sym;
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_displayGameMessage, o_ServerNetworkHandler_displayGameMessage, Hook::ServerNetworkHandler_displayGameMessage);
    HOOKFUNC(threeArgVoid, __fn_Level_onPlayerDeath, o_Level_onPlayerDeath, Hook::Level_onPlayerDeath);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_handle_PlayerAuthInputPacketName, o_ServerNetworkHandler_handle_PlayerAuthInputPacket, Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket);

    BIND(ulong(void*), __fn_NetworkIdentifier_getHash, o_NetworkIdentifier_getHash);
}

subhook::Hook *Hook::getHook(const std::string &name) {
    try {
        return this->nameToHook.at(name);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

template <typename T> bool Hook::bind(const std::string &name, std::function<T> &hook) {
    std::cout << "Binding " << name << std::endl;
    hook = (T *)this->symMap.getFunction(name);
    return hook != nullptr;
}

template<typename T>
bool Hook::hookFunc(const std::string &name, const std::string &hname, std::function<T> &hookOrigFn, void *target) {
    auto shortName = hname.substr(6); // Strip hook:: from name
    std::cout << "Hooking " << name << " to " << shortName << std::endl;
    auto ptr = this->symMap.getFunction(name);
    hookOrigFn = (T *)ptr;
    if (hookOrigFn != nullptr) {
        auto h = new subhook::Hook();
        if (!h->Install((void *) ptr, (void *) target, subhook::HookFlag64BitOffset)) {
            std::cerr << "Error hooking " << name << std::endl;
            delete h;
            return false;
        }
        this->hooks.push_back(h);
        this->nameToHook[shortName] = h;
        return true;
    }

    return false;
}