//
// Created by lucas on 28/11/2021.
//

#include <symbolmap.h>
#include <iostream>
#include <log.h>
#include <fstream>

#include "hook.h"
#include "hookNames.h"

#define HOOKFUNC(type, symbol, source, target) (hookFunc<type>(symbol, #target, source, (void *)target))
#define BIND(type, symbol, target) (bind<type>(symbol, target))

Hook *Hook::singleton;

Hook::Hook() = default;

void Hook::InitSingleton(const SymbolMap &map) {
    Log::Info("Initializing hooks\n");
    singleton = new Hook();
    singleton->Init(map);
    singleton->LoadPlayerDeaths();
    singleton->stopped = false;
}

Hook::~Hook() {
    for (const auto& h: this->hooks) {
        h->Remove();
    }
}

void Hook::Init(const SymbolMap &sym) {
    symMap = sym;
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_displayGameMessage, o_ServerNetworkHandler_displayGameMessage, Hook::ServerNetworkHandler_displayGameMessage);
    HOOKFUNC(threeArgVoid, __fn_Level_onPlayerDeath, o_Level_onPlayerDeath, Hook::Level_onPlayerDeath);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_handle_PlayerAuthInputPacketName, o_ServerNetworkHandler_handle_PlayerAuthInputPacket, Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_handle_ModalFormResponsePacket, o_ServerNetworkHandler_handle_ModalFormResponsePacket, Hook::ServerNetworkHandler_handle_ModalFormResponsePacket);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_onClientAuthenticated, o_ServerNetworkHandler_onClientAuthenticated, Hook::ServerNetworkHandler_onClientAuthenticated);
    HOOKFUNC(threeArgVoid, __fn_ServerNetworkHandler_onPlayerLeft, o_ServerNetworkHandler_onPlayerLeft, Hook::ServerNetworkHandler_onPlayerLeft);
    HOOKFUNC(oneArgVoid, __fn_ServerNetworkHandler_onTick, o_ServerNetworkHandler_onTick, Hook::ServerNetworkHandler_onTick);
    HOOKFUNC(twoArgVoid, __fn_StartGamePacket_WriteBinaryStream, o_StartGamePacket_writeBinaryStream, Hook::StartGamePacket_write_BinaryStream);
    HOOKFUNC(threeArgVoid, __fn_Level_playerChangeDimension, o_Level_playerChangeDimension, Hook::Level_playerChangeDimension);

    // BedrockLog::log we hook but dont save the original one
    auto logPtr = this->symMap.getFunction(__fn_BedrockLog_log);
    if (logPtr != nullptr) {
        auto h = std::make_shared<subhook::Hook>();
        if (!h->Install((void *) logPtr, (void *) Hook::bedLog, subhook::HookFlag64BitOffset)) {
            Log::Error("Error hooking log function\n");
        } else {
            this->hooks.push_back(h);
            this->nameToHook["bedLog"] = h;
        }
    }
}

std::shared_ptr<subhook::Hook> Hook::getHook(const std::string &name) {
    try {
        return this->nameToHook.at(name);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

template<typename T>
bool Hook::bind(const std::string &name, std::function<T> &hook) {
    Log::Debug("Binding {}\n", name);
    hook = (T *) this->symMap.getFunction(name);
    if (hook == nullptr) {
        Log::Error("Not found: {}\n", name);
        throw std::exception();
    }
    return hook != nullptr;
}

template<typename T>
bool Hook::hookFunc(const std::string &name, const std::string &hname, std::function<T> &hookOrigFn, void *target) {
    auto shortName = hname.substr(6); // Strip hook:: from name
    Log::Debug("Hooking {} to {}\n", name, shortName);
    auto ptr = this->symMap.getFunction(name);
    hookOrigFn = (T *) ptr;
    if (hookOrigFn != nullptr) {
        auto h = std::make_shared<subhook::Hook>();
        if (!h->Install((void *) ptr, (void *) target, subhook::HookFlag64BitOffset)) {
            Log::Error("Error hooking {}\n", name);
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

const NetworkIdentifier *Hook::getPlayerNetworkIdentifier(const std::string &name) {
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

void Hook::SavePlayerDeaths() {
    Json::Value root;
    for(auto &p: playerDeaths) {
        Log::Debug("Player: {} Deaths: {}\n", p.first, p.second);
        root[p.first] = p.second;
    }

    Json::StyledWriter styledWriter;
    std::ofstream myfile;
    myfile.open ("playerDeaths.json");
    myfile << styledWriter.write(root);
    myfile.close();
    Log::Info("Player Deaths saved!\n");
}

void Hook::LoadPlayerDeaths() {
    std::ifstream myfile;
    myfile.open ("playerDeaths.json");
    if (!myfile.is_open()) {
        Log::Error("cannot load playerDeaths.json\n");
        return;
    }
    Json::Value root;
    myfile >> root;
    myfile.close();



    for (auto &k: root.getMemberNames()) {
        auto v = root[k].asInt();
        Log::Info("Player: {} Deaths: {}\n", k, v);
        playerDeaths[k] = v;
    }
}