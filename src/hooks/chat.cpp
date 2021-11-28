//
// Created by lucas on 28/11/2021.
//
#include <iostream>
#include "hook.h"
#include "hookNames.h"


void *Hook::ServerNetworkHandler_displayGameMessage(void *thisObj, void *player, void *chatEvent) {
    subhook::ScopedHookRemove remove(singleton->getHook(__func__));

    const char *message = *((char **)chatEvent);
    const char *username = ((char *)chatEvent) + 0x68;
    std::cout << "CHAT(" << username << "): " << message << std::endl;

    return singleton->o_ServerNetworkHandler_displayGameMessage(thisObj, player, chatEvent);
}