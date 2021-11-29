//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include "hook.h"

void *Hook::Level_onPlayerDeath(void *thisObj, void *player, void *actorDamageSource) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);
    const char *username = ((char *) player) + 0x900;
    std::cout << "Player " << username << " died" << std::endl;

    return singleton->o_Level_onPlayerDeath(thisObj, player, actorDamageSource);
}

void *Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, void *networkIdentifier, void *playerAuthInputPacket) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);

    auto base = (float *) ((char *) playerAuthInputPacket + 0x2C);
    auto hash = singleton->o_NetworkIdentifier_getHash(networkIdentifier);

    float pitch = base[0];
    float yaw = base[1];
    float X = base[2];
    float Y = base[3];
    float Z = base[4];
    float headYaw = base[5];

//    std::cout << "Hash: " << hash << " X: " << X << " Y: " << Y << " Z: " << Z << std::endl;

    return singleton->o_ServerNetworkHandler_handle_PlayerAuthInputPacket(thisObj, networkIdentifier, playerAuthInputPacket);
}

void *Hook::ServerNetworkHandler_onClientAuthenticated(void *thisObj, void *networkIdentifier, void *certificate) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);
    auto hash = singleton->o_NetworkIdentifier_getHash(networkIdentifier);
    std::string username, xuid;

    singleton->o_ExtendedCertificate_getIdentityName(username, certificate);
    singleton->o_ExtendedCertificate_getXuid(xuid, certificate);

    std::cout << "User " << username << " (" << xuid << ") logged in." << std::endl;

    singleton->hashToNetworkIdentifier[hash] = (NetworkIdentifier*)networkIdentifier;
    singleton->playerToHash[username] = hash;

    return singleton->o_ServerNetworkHandler_onClientAuthenticated(thisObj, networkIdentifier, certificate);
}

void *Hook::ServerNetworkHandler_onPlayerLeft(void *thisObj, void *serverPlayer, void *something) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);

    const char *username = ((char *) serverPlayer) + 0x900;
    std::cout << "Player " << username << " left" << std::endl;

    try {
        auto hash = singleton->playerToHash.at(username);
        singleton->playerToHash.erase(username);
        singleton->hashToNetworkIdentifier.erase(hash);
    } catch (const std::out_of_range &) {
        // Player was not in table
    }

    return singleton->o_ServerNetworkHandler_onPlayerLeft(thisObj, serverPlayer, something);
}