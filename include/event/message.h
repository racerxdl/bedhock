//
// Created by lucas on 29/11/2021.
//

#pragma once

#include "event.h"

enum class MessageType {
    NORMAL = 0,
    SYSTEM = 1,
    ANOUNCE = 2,
    WHISPER = 3,
    JUKEBOX = 4,
};

class MessageEvent : public HockEvent {
public:
    MessageEvent() : MessageEvent(MessageType::NORMAL, "", "", "") {}

    MessageEvent(MessageType messageType, std::string const &message) : MessageEvent(messageType, "", "", message) {}

    MessageEvent(MessageType messageType, std::string const &from, std::string const &message) : MessageEvent(messageType, from, "", message) {}

    MessageEvent(MessageType messageType, std::string from, std::string to, std::string message) : HockEvent(HockEventType::EVENT_MESSAGE), msgType((int) messageType), from(std::move(from)),
                                                                                                   to(std::move(to)), message(std::move(message)), translatable(false) {}

    std::string from, to, message;
    int msgType;
    bool translatable;

    [[nodiscard]]
    bool isDirected() const {
        return !to.empty() && ((msgType == (int) MessageType::WHISPER) || msgType == (int) MessageType::NORMAL);
    }

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["msgType"] = msgType;
        root["from"] = from;
        root["to"] = to;
        root["message"] = message;
        root["translatable"] = translatable;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        msgType = root.get("msgType", (int) MessageType::NORMAL).asInt();
        from = root.get("from", "").asString();
        to = root.get("to", "").asString();
        message = root.get("message", "").asString();
        translatable = root.get("translatable", false).asBool();
    }
};