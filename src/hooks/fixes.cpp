//
// Created by lucas on 30/11/2021.
//

#include <cmath>
#include <iostream>
#include <fmt/format.h>
#include "hook.h"

void *Hook::StartGamePacket_write_BinaryStream(void *thisObj, void *binaryStream) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    // This fixes a memory-leak that can happen client-side when Pitch/Yaw are NaN
    // This seens to happen only with Android Version, when in specific scenarios
    // under water. But its pretty easy to simulate by using MCPE-Linux and just going
    // to water and waiting for the drowing animation.
    // So if any of these floats are nan, we set to zero.
    // This does not prevent the problem in client, but at least it doesnt lock-up
    // the player.

    auto base = (float*)(((char *)thisObj) + 1308);
    for (int i = 0; i < 5; i++) {
        if (std::isnan(base[i])) {
            fmt::print("Fixing bug for client. Float {} was NaN. ", i);
            base[i] = 0;
        }
    }

    return singleton->o_StartGamePacket_writeBinaryStream(thisObj, binaryStream);
}