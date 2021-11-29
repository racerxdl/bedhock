//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include <chrono>
#include <wrap/TextPacket.h>
#include "hook.h"
#include "fakemine/fakemine.h"

auto lastSend = std::chrono::system_clock::now();

void *Hook::ServerNetworkHandler_onTick(void *thisObj) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);
    auto handler = (ServerNetworkHandler *) thisObj;
    // TICK
    auto delta = std::chrono::duration_cast<std::chrono::seconds>((std::chrono::system_clock::now() - lastSend));

    if (delta.count() >= 5) {
        WrappedTextPacket packet(singleton);
        packet.createChat("Test0", "test1", "test2", "test3");
        std::cout << "Sending packet" << std::endl;
        handler->packetSender->sendBroadcast(packet.get());
        lastSend = std::chrono::system_clock::now();
    }

    return singleton->o_ServerNetworkHandler_onTick(thisObj);
}