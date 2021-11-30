//
// Created by lucas on 28/11/2021.
//

#include <symbolmap.h>
#include <iostream>

#include "hook.h"
#include "hookNames.h"

#define HOOKFUNC(type, symbol, source, target) (hookFunc<type>(symbol, #target, source, (void *)target))
#define BIND(type, symbol, target) (bind<type>(symbol, target))

Hook *Hook::singleton;

Hook::Hook() = default;

void Hook::InitSingleton(const SymbolMap &map) {
    std::cout << "Initializing hooks" << std::endl;
    singleton = new Hook();
    singleton->Init(map);
    singleton->stopped = false;
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
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_onClientAuthenticated, o_ServerNetworkHandler_onClientAuthenticated, Hook::ServerNetworkHandler_onClientAuthenticated);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_onPlayerLeft, o_ServerNetworkHandler_onPlayerLeft, Hook::ServerNetworkHandler_onPlayerLeft);
    HOOKFUNC(oneArgVoid, __fn_ServerNetworkHandler_onTick, o_ServerNetworkHandler_onTick, Hook::ServerNetworkHandler_onTick);
    HOOKFUNC(twoArgVoid, __fn_StartGamePacket_WriteBinaryStream, o_StartGamePacket_writeBinaryStream, Hook::StartGamePacket_write_BinaryStream);

    BIND(ulong(void * ), __fn_NetworkIdentifier_getHash, o_NetworkIdentifier_getHash);
    BIND(void (std::string&, void *), __fn_ExtendedCertificate_getIdentityName, o_ExtendedCertificate_getIdentityName);
    BIND(void (std::string&, void *), __fn_ExtendedCertificate_getXuid, o_ExtendedCertificate_getXuid);
    BIND(void(void * , std::string, std::string, std::string, std::string), __fn_TextPacket_createChat, o_TextPacket_createChat);
    BIND(void(void * ), __fn_TextPacket_TextPacket, o_TextPacket_TextPacket);
    BIND(void(void * ), __fn_TextPacket_destructor, o_TextPacket_destructor);

    BIND(void(void * , std::string), __fn_TextPacket_createRaw, o_TextPacket_createRaw);
    BIND(void(void * , std::string, std::vector<std::string>), __fn_TextPacket_createTranslated, o_TextPacket_createTranslated);
    BIND(void(void * , std::string), __fn_TextPacket_createSystemMessage, o_TextPacket_createSystemMessage);
    BIND(void(void * , std::string const&, std::string const&, std::string const&, std::string const&), __fn_TextPacket_createAnnouncement, o_TextPacket_createAnnouncement);
    BIND(void(void * , std::string const&, std::string const&, std::string const&, std::string const&), __fn_TextPacket_createTranslatedAnnouncement, o_TextPacket_createTranslatedAnnouncement);
    BIND(void(void * , std::string const&, std::string const&, std::string const&, std::string const&), __fn_TextPacket_createTranslatedChat, o_TextPacket_createTranslatedChat);
    BIND(void(void * , std::string const&, std::vector<std::string> const&), __fn_TextPacket_createJukeboxPopup, o_TextPacket_createJukeboxPopup);
    BIND(void(void * , std::string const&, std::string const&, std::string const&, std::string const&), __fn_TextPacket_createWhisper, o_TextPacket_createWhisper);
}

subhook::Hook *Hook::getHook(const std::string &name) {
    try {
        return this->nameToHook.at(name);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

template<typename T>
bool Hook::bind(const std::string &name, std::function<T> &hook) {
    std::cout << "Binding " << name << std::endl;
    hook = (T *) this->symMap.getFunction(name);
    return hook != nullptr;
}

template<typename T>
bool Hook::hookFunc(const std::string &name, const std::string &hname, std::function<T> &hookOrigFn, void *target) {
    auto shortName = hname.substr(6); // Strip hook:: from name
    std::cout << "Hooking " << name << " to " << shortName << std::endl;
    auto ptr = this->symMap.getFunction(name);
    hookOrigFn = (T *) ptr;
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

void *Hook::getFunctionPointer(const std::string &name) {
    return singleton->symMap.getFunction(name);
}

NetworkIdentifier *Hook::getPlayerNetworkIdentifier(const std::string &name) {
    try {
        auto hash = playerToHash.at(name);
        return hashToNetworkIdentifier.at(hash);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

bool Hook::ReadInputEvent(std::shared_ptr<HockEvent> &event) {
    return singleton->input.Consume(event);
}

bool Hook::ReadOutputEvent(std::shared_ptr<HockEvent> &event) {
    return singleton->output.Consume(event);
}
void Hook::WriteInputEvent(std::shared_ptr<HockEvent> event) {
    if (singleton->input.Size() >= MAX_QUEUE_LENGTH) {
        auto discard = std::make_shared<HockEvent>();
        std::ignore = singleton->input.Consume(discard); // Discard one
    }
    singleton->input.Produce(std::move(event));
}
void Hook::WriteOutputEvent(std::shared_ptr<HockEvent> event) {
    if (singleton->output.Size() >= MAX_QUEUE_LENGTH) {
        auto discard = std::make_shared<HockEvent>();
        std::ignore = singleton->output.Consume(discard); // Discard one
    }
    singleton->output.Produce(std::move(event));
}

std::vector<std::string> Hook::playerList() {
    std::vector<std::string> players;
    for (const auto& p: playerToHash) {
        players.push_back(p.first);
    }
    return players;
}

void Hook::StopAll() {
    singleton->input.Finish();
    singleton->output.Finish();
    singleton->stopped = true;
}