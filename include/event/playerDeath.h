#pragma once

#include "event.h"

class PlayerDeathCountRequestEvent : public HockEvent {
   public:
    PlayerDeathCountRequestEvent() : HockEvent(HockEventType::EVENT_PLAYER_DEATH_COUNT_REQUEST) {}
};

class PlayerDeathCountResponseEvent : public HockEvent {
   public:
    std::map<std::string, int> playerDeaths;

    PlayerDeathCountResponseEvent() : HockEvent(HockEventType::EVENT_PLAYER_DEATH_COUNT_RESPONSE) {}
    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        Json::Value pd;
        for (const auto &kv : playerDeaths) {
            pd[kv.first] = kv.second;
        }
        root["playerDeaths"] = pd;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        Json::Value pd = root["playerDeaths"];
        for (auto &k : pd.getMemberNames()) {
            auto v = pd[k].asInt();
            playerDeaths[k] = v;
        }
    }
};
