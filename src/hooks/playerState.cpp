//
// Created by lucas on 28/11/2021.
//

#include <event/message.h>
#include <event/playerupdate.h>
#include <fmt/format.h>

#include <cmath>
#include <cstring>
#include <iostream>

#include "fakemine/NetworkIdentifier.h"
#include "fakemine/binaryStream.h"
#include "hook.h"

void *Hook::Level_playerChangeDimension(void *thisObj, Player *player, ChangeDimensionRequest *changeDimensionRequest) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto fromDimensionId = changeDimensionRequest->FromDimensionId;
    auto toDimensionId = changeDimensionRequest->ToDimensionId;
    auto username = player->getName();

    std::string fromDimensionName, toDimensionName;
    auto xuid = singleton->playerToXuid[username];

    switch (fromDimensionId) {
        case 0:
            fromDimensionName = "overworld";
            break;
        case 1:
            fromDimensionName = "nether";
            break;
        case 2:
            fromDimensionName = "the end";
            break;
        default:
            fromDimensionName = "unknown";
    }
    switch (toDimensionId) {
        case 0:
            toDimensionName = "overworld";
            break;
        case 1:
            toDimensionName = "nether";
            break;
        case 2:
            toDimensionName = "the end";
            break;
        default:
            toDimensionName = "unknown";
    }
    if (changeDimensionRequest->State == 0) {
        Log::Info("Player {} went from {} to {}\n", username, fromDimensionName, toDimensionName);
        WriteOutputEvent(std::make_shared<PlayerChangeDimensionEvent>(username, xuid, fromDimensionId, toDimensionId));
    }
    return singleton->o_Level_playerChangeDimension(thisObj, player, changeDimensionRequest);
}

void *Hook::Level_onPlayerDeath(void *thisObj, Player *player, ActorDamageSource *actorDamageSource) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());
    auto username = player->getName();
    auto cause = ActorDamageSource::lookupCauseName(actorDamageSource->getCause());
    Log::Info("Player {} died by {}\n", username, cause);

    auto xuid = singleton->playerToXuid[username];
    auto p = std::make_shared<PlayerDeathEvent>(username, xuid, cause);
    WriteOutputEvent(p);

    return singleton->o_Level_onPlayerDeath(thisObj, player, actorDamageSource);
}

void *Hook::ServerNetworkHandler_handle_PlayerAuthInputPacket(void *thisObj, NetworkIdentifier *networkIdentifier, void *playerAuthInputPacket) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto base = (float *)((char *)playerAuthInputPacket + 0x2C);
    auto hash = networkIdentifier->getHash();
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

void *Hook::ServerNetworkHandler_onClientAuthenticated(void *thisObj, const NetworkIdentifier &networkIdentifier, const Certificate &certificate) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    auto hash = networkIdentifier.getHash();
    auto username = ExtendedCertificate::getIdentityName(certificate);
    auto xuid = ExtendedCertificate::getXuid(certificate);

    Log::Info("User {} ({}) logged in\n", username, xuid);

    singleton->hashToNetworkIdentifier[hash] = &networkIdentifier;
    singleton->playerToHash[username] = hash;
    singleton->playerToXuid[username] = xuid;
    singleton->hashToPlayer[hash] = username;

    auto p = std::make_shared<PlayerJoinEvent>(username, xuid);
    WriteOutputEvent(p);

    return singleton->o_ServerNetworkHandler_onClientAuthenticated(thisObj, (void *)&networkIdentifier, (void *)&certificate);
}

void *Hook::ServerNetworkHandler_onPlayerLeft(void *thisObj, ServerPlayer *serverPlayer, void *something) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());
    auto username = serverPlayer->getName();
    auto xuid = singleton->playerToXuid[username];
    Log::Info("Player {} ({}) left.\n", username, xuid);
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