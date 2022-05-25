//
// Created by lucas on 29/11/2021.
//

#pragma once

#include <json/serializable.h>
#include <jsoncpp/json/json.h>

enum class HockEventType {
    EVENT_INVALID = -1,
    EVENT_MESSAGE = 0,
    EVENT_PLAYER_JOIN = 1,
    EVENT_PLAYER_LEFT = 2,
    EVENT_PLAYER_DEATH = 3,
    EVENT_PLAYER_UPDATE = 4,
    EVENT_PLAYER_LIST = 5,
    EVENT_PLAYER_DIMENSION_CHANGE = 6,
    EVENT_LOG = 7,
    EVENT_FORM_REQUEST = 8,
    EVENT_FORM_RESPONSE = 9,
};

class HockEvent : public IJsonSerializable {
   private:
    int type;

   protected:
   public:
    explicit HockEvent() : HockEvent(HockEventType::EVENT_INVALID) {}

    explicit HockEvent(HockEventType type) : type((int)type) {}

    [[nodiscard]] HockEventType getType() const { return (HockEventType)type; }

    void Serialize(Json::Value &root) override {
        root["type"] = type;
    }

    void Deserialize(Json::Value &root) override {
        type = root.get("type", (int)HockEventType::EVENT_INVALID).asInt();
    }
};