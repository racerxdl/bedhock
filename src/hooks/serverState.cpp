//
// Created by lucas on 28/11/2021.
//
#include <fmt/format.h>
#include <wrap/Server.h>
#include <event/form.h>
#include "hook.h"

#define MAX_QUEUE_PROC 64

void *Hook::ServerNetworkHandler_onTick(void *thisObj) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

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

void *Hook::ServerNetworkHandler_handle_ModalFormResponsePacket(void *thisObj, NetworkIdentifier *networkIdentifier, ModalFormResponsePacket *modalFormResponsePacket) {
    static std::shared_ptr<subhook::Hook> hook;
    if (hook == nullptr) {
        hook = singleton->getHook(__func__);
    }
    subhook::ScopedHookRemove remove(hook.get());

    WrappedServer server(singleton, thisObj);

    auto player = server.getServerPlayer(*networkIdentifier);
    auto name = player->getName();

    WriteOutputEvent(std::make_shared<FormResponseEvent>(modalFormResponsePacket->formId, modalFormResponsePacket->data, name));

    return singleton->o_ServerNetworkHandler_handle_ModalFormResponsePacket(thisObj, networkIdentifier, modalFormResponsePacket);
}