#include <cstdio>
#include <subhook.h>
#include <iostream>
#include <string>

#include "tools.h"
#include "symbolmap.h"
#include "hook.h"

//#define HOOK_ADDR 0x19b1640

//typedef void *(*_displayGameMessage)(void *thisobj, void *player, void *chat);
//
//_displayGameMessage displayGameMessage = (_displayGameMessage) HOOK_ADDR;
//
//subhook::Hook *hooktest;
//
//// ServerNetworkHandler::_displayGameMessage(Player const&, ChatEvent&)
//void *hookfn(void *thisobj, void *player, void *chat) {
//    subhook::ScopedHookRemove remove(hooktest);
//
//    const char *message = *((char **)chat);
//    const char *username = ((char *)chat) + 0x68;
//    std::cout << "CHAT(" << username << "): " << message << std::endl;
//
//    void *res = displayGameMessage(thisobj, player, chat);
//
//    return res;
//}


static struct init {
    init() {
        std::string execName = currentExecutableName();
        std::cout << "Executable: " << execName << std::endl;

        SymbolMap symMap(execName);
        Hook::InitSingleton(symMap);

//        hooktest = new subhook::Hook();
//        if (!hooktest->Install((void *)HOOK_ADDR, (void *)&hookfn, subhook::HookFlag64BitOffset)) {
//            std::cout << "Failed to hook" << std::endl;
//            exit(1);
//        }
        std::cout << "Bedhock Initialization done" << std::endl;
    }

    ~init() {
        std::cout << "Cleaning up" << std::endl;
    }
} cpp_bedhook_init;
