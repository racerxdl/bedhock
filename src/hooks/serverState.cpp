//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include <chrono>
#include <wrap/Server.h>
#include "hook.h"

auto lastSend = std::chrono::system_clock::now();

void *Hook::ServerNetworkHandler_onTick(void *thisObj) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);
    WrappedServer server(singleton, thisObj);
    auto delta = std::chrono::duration_cast<std::chrono::seconds>((std::chrono::system_clock::now() - lastSend));

    if (delta.count() >= 5) {
        server.broadcastMessage("The Server", "The Message");

        for (auto const &p: singleton->playerToHash) {
            server.sendMessageTo("Server", p.first, "Henlo "  + p.first);
        }

        lastSend = std::chrono::system_clock::now();
    }

    return singleton->o_ServerNetworkHandler_onTick(thisObj);
}