//
// Created by lucas on 12/12/2021.
//

#pragma once

#include <utility>

#include "event.h"

class LogEvent : public HockEvent {
public:
    LogEvent() : LogEvent(0, 0, 0, 0, 0, "", 0, "") {}

    LogEvent(int category, int bitset, int rules, int area, int unk0, std::string functionName, int functionLine, std::string message) :
            HockEvent(HockEventType::EVENT_LOG), category(category), bitset(bitset), rules(rules), area(area), unk0(unk0), functionLine(functionLine), functionName(std::move(functionName)), message(std::move(message)) {}

    int category;
    int bitset;
    int rules;
    int area;
    int unk0;
    int functionLine;
    std::string functionName;
    std::string message;

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["category"] = category;
        root["bitset"] = bitset;
        root["rules"] = rules;
        root["area"] = area;
        root["unk0"] = unk0;
        root["functionLine"] = functionLine;
        root["functionName"] = functionName;
        root["message"] = message;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        category = root.get("category", 0).asInt();
        bitset = root.get("bitset", 0).asInt();
        rules = root.get("rules", 0).asInt();
        area = root.get("area", 0).asInt();
        functionLine = root.get("functionLine", 0).asInt();
        functionName = root.get("functionName", "").asString();
        message = root.get("message", "").asString();
    }
};