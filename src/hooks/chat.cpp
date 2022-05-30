//
// Created by lucas on 28/11/2021.
//
#include <event/message.h>
#include <fmt/format.h>

#include <iostream>

#include "hook.h"

void *Hook::ServerNetworkHandler_displayGameMessage(void *thisObj, void *_player, void *_chatEvent) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());
    Player *player = (Player *) _player;
    ChatEvent *chatEvent = (ChatEvent *) _chatEvent;

    auto message = chatEvent->message;
    auto username = player->getName();

    WriteOutputEvent(std::make_shared<MessageEvent>(MessageType::NORMAL, username, message));

    Log::Info("[CHAT] {}: {}\n", username, message);

    return singleton->o_ServerNetworkHandler_displayGameMessage(thisObj, player, chatEvent);
}