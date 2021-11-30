//
// Created by lucas on 29/11/2021.
//

#pragma once

#include "event.h"

class PlayerListEvent : public HockEvent {
public:
    PlayerListEvent() : HockEvent(HockEventType::EVENT_PLAYER_LIST) {}
    std::vector<std::string> players;

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["players"] = Json::Value(Json::arrayValue);
        for (int i = 0; i < players.size(); i++) {
            root["players"][i] = players[i];
        }
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        auto p = root["players"];
        for (const auto & i : p) {
            players.push_back(i.asString());
        }
    }
};