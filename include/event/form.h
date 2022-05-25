#pragma once

#include "event.h"

class FormRequestEvent : public HockEvent {
   public:
    unsigned int formId;
    std::string jsonData;
    std::string to;

    FormRequestEvent() : FormRequestEvent(0, "", "") {}
    FormRequestEvent(unsigned int formId, const std::string &jsonData, const std::string &to = "") : HockEvent(HockEventType::EVENT_FORM_REQUEST), formId(formId), jsonData(jsonData), to(to) {}

    [[nodiscard]] bool isDirected() const {
        return !to.empty();
    }

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["formId"] = formId;
        root["jsonData"] = jsonData;
        root["to"] = to;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        formId = root.get("formId", 0).asInt();
        jsonData = root.get("jsonData", "").asString();
        to = root.get("to", "").asString();
    }
};

class FormResponseEvent : public HockEvent {
   public:
    unsigned int formId;
    std::string response;
    std::string from;

    FormResponseEvent() : FormResponseEvent(0, "", "") {}
    FormResponseEvent(unsigned int formId, const std::string &response, const std::string &from) : HockEvent(HockEventType::EVENT_FORM_RESPONSE), formId(formId), response(response), from(from) {}

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["formId"] = formId;
        root["response"] = response;
        root["from"] = from;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        formId = root.get("formId", 0).asInt();
        response = root.get("response", "").asString();
        from = root.get("from", "").asString();
    }
};
