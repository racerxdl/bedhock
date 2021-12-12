//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include <event/playerupdate.h>
#include <cmath>
#include "hook.h"

void *Hook::Level_playerChangeDimension(void *thisObj,  void *player, ChangeDimensionRequest *changeDimensionRequest) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto dimensionId = changeDimensionRequest->dimensionId;
    auto username = ((const char *) player) + 0x8F8;
    std::string dimensionName;
    auto xuid = singleton->playerToXuid[username];

    switch (dimensionId) {
        case 0: dimensionName = "overworld"; break;
        case 1: dimensionName = "nether"; break;
        case 2: dimensionName = "the end"; break;
        default: dimensionName = "unknown";
    }

    // std::cout << "Player " << username << " went to " << dimensionName << std::endl;

    WriteOutputEvent(std::make_shared<PlayerChangeDimensionEvent>(username, xuid, dimensionId));

    return singleton->o_Level_playerChangeDimension(thisObj, player, changeDimensionRequest);
}

void *Hook::Level_onPlayerDeath(void *thisObj, void *player, void *actorDamageSource) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());
    const char *username = ((char *) player) + 0x8F8;
    std::cout << "Player " << username << " died" << std::endl;

    auto xuid = singleton->playerToXuid[username];
    auto p = std::make_shared<PlayerDeathEvent>(username, xuid);
    WriteOutputEvent(p);

    return singleton->o_Level_onPlayerDeath(thisObj, player, actorDamageSource);
}

void *Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, void *networkIdentifier, void *playerAuthInputPacket) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto base = (float *) ((char *) playerAuthInputPacket + 0x2C);
    auto hash = singleton->o_NetworkIdentifier_getHash(networkIdentifier);
    auto username = singleton->hashToPlayer[hash];
    auto xuid = singleton->playerToXuid[username];

    float pitch = base[0];
    float yaw = base[1];
    float X = base[2];
    float Y = base[3];
    float Z = base[4];
    float headYaw = base[5];

    // See fixes.cpp for why these checks are done here.
    if (std::isnan(pitch)) {
        base[0] = 0;
    }
    if (std::isnan(yaw)) {
        base[1] = 0;
    }
    if (std::isnan(headYaw)) {
        base[5] = 0;
    }

    auto p = std::make_shared<PlayerUpdate>(username, xuid, pitch, yaw, X, Y, Z, headYaw);
    WriteOutputEvent(p);

    return singleton->o_ServerNetworkHandler_handle_PlayerAuthInputPacket(thisObj, networkIdentifier, playerAuthInputPacket);
}

void *Hook::ServerNetworkHandler_onClientAuthenticated(void *thisObj, void *networkIdentifier, void *certificate) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto hash = singleton->o_NetworkIdentifier_getHash(networkIdentifier);
    std::string username, xuid;

    singleton->o_ExtendedCertificate_getIdentityName(username, certificate);
    singleton->o_ExtendedCertificate_getXuid(xuid, certificate);

    std::cout << "User " << username << " (" << xuid << ") logged in." << std::endl;

    singleton->hashToNetworkIdentifier[hash] = (NetworkIdentifier*)networkIdentifier;
    singleton->playerToHash[username] = hash;
    singleton->playerToXuid[username] = xuid;
    singleton->hashToPlayer[hash] = username;

    auto p = std::make_shared<PlayerJoinEvent>(username, xuid);
    WriteOutputEvent(p);

    return singleton->o_ServerNetworkHandler_onClientAuthenticated(thisObj, networkIdentifier, certificate);
}

void *Hook::ServerNetworkHandler_onPlayerLeft(void *thisObj, void *serverPlayer, void *something) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    const char *username = ((char *) serverPlayer) + 0x8F8;
    std::cout << "Player " << username << " left" << std::endl;

    auto xuid = singleton->playerToXuid[username];
    auto p = std::make_shared<PlayerLeftEvent>(username, xuid);
    WriteOutputEvent(p);

    try {
        auto hash = singleton->playerToHash.at(username);
        singleton->playerToHash.erase(username);
        singleton->playerToXuid.erase(username);
        singleton->hashToNetworkIdentifier.erase(hash);
        singleton->hashToPlayer.erase(hash);
    } catch (const std::out_of_range &) {
        // Player was not in table
    }

    return singleton->o_ServerNetworkHandler_onPlayerLeft(thisObj, serverPlayer, something);
}