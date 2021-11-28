//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include "hook.h"

void *Hook::Level_onPlayerDeath(void *thisObj, void *player, void *actorDamageSource) {
    subhook::ScopedHookRemove remove(singleton->getHook(__func__));
    const char *username = ((char *) player) + 0x900;
    std::cout << "Player " << username << " died" << std::endl;

    return singleton->o_Level_onPlayerDeath(thisObj, player, actorDamageSource);
}

void *Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, void *networkIdentifier, void *playerAuthInputPacket) {
    subhook::ScopedHookRemove remove(singleton->getHook(__func__));
    auto base = (float *) ((char *) playerAuthInputPacket + 0x2C);
    auto hash = singleton->o_NetworkIdentifier_getHash(networkIdentifier);

    float pitch = base[0];
    float yaw = base[1];
    float X = base[2];
    float Y = base[3];
    float Z = base[4];
    float headYaw = base[5];

//    if (hash == 0) {
        std::cout << "Hash: " << hash << " X: " << X << " Y: " << Y << " Z: " << Z << std::endl;
//    }

    return singleton->o_ServerNetworkHandler_handle_PlayerAuthInputPacket(thisObj, networkIdentifier, playerAuthInputPacket);
}