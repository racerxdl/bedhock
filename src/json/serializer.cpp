//
// Created by lucas on 29/11/2021.
//

#include "json/serializer.h"

#include <event/form.h>
#include <event/message.h>
#include <event/playerlist.h>
#include <event/playerupdate.h>
#include <event/scoreboard.h>
#include <event/playerDeath.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

bool CJsonSerializer::Serialize(IJsonSerializable *pObj, std::string &output) {
    if (pObj == nullptr)
        return false;

    Json::Value serializeRoot;
    pObj->Serialize(serializeRoot);

    Json::StyledWriter writer;
    output = writer.write(serializeRoot);

    return true;
}

bool CJsonSerializer::Deserialize(IJsonSerializable *pObj, std::string &input) {
    if (pObj == nullptr)
        return false;

    Json::Value deserializeRoot;
    Json::Reader reader;

    if (!reader.parse(input, deserializeRoot))
        return false;

    pObj->Deserialize(deserializeRoot);

    return true;
}

std::shared_ptr<HockEvent> CJsonSerializer::DeserializeEvent(const std::string &input) {
    Json::Value deserializeRoot;
    Json::Reader reader;

    if (!reader.parse(input, deserializeRoot)) {
        return nullptr;
    }

    std::shared_ptr<HockEvent> event;
    int type = deserializeRoot.get("type", (int)HockEventType::EVENT_INVALID).asInt();
    switch ((HockEventType)type) {
        case HockEventType::EVENT_MESSAGE:
            event = std::make_shared<MessageEvent>();
            break;
        case HockEventType::EVENT_PLAYER_JOIN:
            event = std::make_shared<PlayerJoinEvent>();
            break;
        case HockEventType::EVENT_PLAYER_LEFT:
            event = std::make_shared<PlayerLeftEvent>();
            break;
        case HockEventType::EVENT_PLAYER_DEATH:
            event = std::make_shared<PlayerDeathEvent>();
            break;
        case HockEventType::EVENT_PLAYER_UPDATE:
            event = std::make_shared<PlayerUpdate>();
            break;
        case HockEventType::EVENT_PLAYER_LIST:
            event = std::make_shared<PlayerListEvent>();
            break;
        case HockEventType::EVENT_FORM_REQUEST:
            event = std::make_shared<FormRequestEvent>();
            break;
        case HockEventType::EVENT_FORM_RESPONSE:
            event = std::make_shared<FormResponseEvent>();
            break;
        case HockEventType::EVENT_SCOREBOARD:
            event = std::make_shared<ScoreBoardEvent>();
            break;
        case HockEventType::EVENT_PLAYER_DEATH_COUNT_REQUEST:
            event = std::make_shared<PlayerDeathCountRequestEvent>();
            break;
        case HockEventType::EVENT_PLAYER_DEATH_COUNT_RESPONSE:
            event = std::make_shared<PlayerDeathCountResponseEvent>();
            break;
        default:
            event = nullptr;
    }

    if (event != nullptr) {
        event->Deserialize(deserializeRoot);
    }

    return event;
}