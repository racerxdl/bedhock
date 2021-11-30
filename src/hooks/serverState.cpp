//
// Created by lucas on 28/11/2021.
//

#include <iostream>
#include "event.h"
#include <chrono>
#include <wrap/Server.h>
#include "hook.h"

#define MAX_QUEUE_PROC 16

void *Hook::ServerNetworkHandler_onTick(void *thisObj) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);
    WrappedServer server(singleton, thisObj);

    auto event = std::make_shared<HockEvent>();
    for (int i = 0; i < MAX_QUEUE_PROC; i++) {
        if (ReadInputEvent(event)) {
            server.handleEvent(event);
        } else {
            break;
        }
    }

    return singleton->o_ServerNetworkHandler_onTick(thisObj);
}