//
// Created by lucas on 28/11/2021.
//
#include <iostream>
#include "hook.h"

void *Hook::ServerNetworkHandler_displayGameMessage(void *thisObj, void *player, void *chatEvent) {
    static subhook::Hook *hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook);

    const char *message = *((char **)chatEvent);
    const char *username = ((char *)chatEvent) + 0x68;
    std::cout << "CHAT(" << username << "): " << message << std::endl;

    return singleton->o_ServerNetworkHandler_displayGameMessage(thisObj, player, chatEvent);
}