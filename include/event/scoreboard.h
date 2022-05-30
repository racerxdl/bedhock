#pragma once

#include "event.h"

struct ScoreBoardEntry : public IJsonSerializable {
    int64_t entryId;
    uint8_t type;
    uint64_t pid;
    uint32_t score;
    int64_t actorId;
    std::string entryName;

    void Serialize(Json::Value &root) override {
        root["entryId"] = entryId;
        root["type"] = type;
        root["pid"] = pid;
        root["score"] = score;
        root["actorId"] = actorId;
        root["entryName"] = entryName;
    }

    void Deserialize(Json::Value &root) override {
        entryId = root.get("entryId", 0).asInt64();
        type = (uint8_t)root.get("type", "").asUInt();
        pid = root.get("pid", "").asUInt64();
        score = root.get("score", "").asUInt();
        actorId = root.get("actorId", "").asInt64();
        entryName = root.get("entryName", "").asString();
    }
};

class ScoreBoardEvent : public HockEvent {
   public:
    std::string name;
    std::string type;
    std::string title;
    std::string to;
    bool sortDescending;
    std::vector<ScoreBoardEntry> entries;

    ScoreBoardEvent() : ScoreBoardEvent(0, "", "") {}
    ScoreBoardEvent(const std::string &name, const std::string &type, const std::string &title, const std::string &to = "") : HockEvent(HockEventType::EVENT_SCOREBOARD), name(name), type(type), title(title), to(to) {}

    [[nodiscard]] bool isDirected() const {
        return !to.empty();
    }

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["name"] = name;
        root["type"] = type;
        root["title"] = title;
        root["to"] = to;
        root["sortDescending"] = sortDescending;
        Json::Value jEntries;

        for (int i = 0; i < entries.size(); i++) {
            entries[i].Serialize(jEntries[i]);
        }
        root["entries"] = jEntries;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        name = root.get("name", 0).asString();
        type = root.get("type", "").asString();
        title = root.get("title", "").asString();
        to = root.get("to", "").asString();
        sortDescending = root.get("sortDescending", false).asBool();
        auto e = root.get("entries", Json::Value());
        for (int i = 0; i < e.size(); i++) {
            ScoreBoardEntry entry;
            entry.Deserialize(e[i]);
            entries.push_back(entry);
        }
    }
};
