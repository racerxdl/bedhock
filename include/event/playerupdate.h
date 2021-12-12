//
// Created by lucas on 29/11/2021.
//

#pragma once

#include <utility>

#include "event.h"

class PlayerStatusEvent : public HockEvent {
protected:
    explicit PlayerStatusEvent(HockEventType type) : HockEvent(type) {}
    PlayerStatusEvent(HockEventType type, std::string username, std::string xuid) : HockEvent(type), username(std::move(username)), xuid(std::move(xuid)) {}

public:
    std::string username, xuid;

    void Serialize(Json::Value &root) override {
        HockEvent::Serialize(root);
        root["username"] = username;
        root["xuid"] = xuid;
    }

    void Deserialize(Json::Value &root) override {
        HockEvent::Deserialize(root);
        username = root["username"].asString();
        xuid = root["xuid"].asString();
    }
};

class PlayerJoinEvent : public PlayerStatusEvent {
public:
    PlayerJoinEvent() : PlayerStatusEvent(HockEventType::EVENT_PLAYER_JOIN) {}
    PlayerJoinEvent(const std::string &username, const std::string &xuid) : PlayerStatusEvent(HockEventType::EVENT_PLAYER_JOIN, username, xuid) {}
};

class PlayerLeftEvent : public PlayerStatusEvent {
public:
    PlayerLeftEvent() : PlayerStatusEvent(HockEventType::EVENT_PLAYER_LEFT) {}
    PlayerLeftEvent(const std::string &username, const std::string &xuid) : PlayerStatusEvent(HockEventType::EVENT_PLAYER_LEFT, username, xuid) {}
};


class PlayerDeathEvent : public PlayerStatusEvent {
public:
    PlayerDeathEvent() : PlayerStatusEvent(HockEventType::EVENT_PLAYER_DEATH) {}
    PlayerDeathEvent(const std::string &username, const std::string &xuid) : PlayerStatusEvent(HockEventType::EVENT_PLAYER_DEATH, username, xuid) {}
};

class PlayerChangeDimensionEvent : public PlayerStatusEvent {
public:
    PlayerChangeDimensionEvent() : PlayerStatusEvent(HockEventType::EVENT_PLAYER_DIMENSION_CHANGE) {}
    PlayerChangeDimensionEvent(const std::string &username, const std::string &xuid, int dimension) : PlayerStatusEvent(HockEventType::EVENT_PLAYER_DIMENSION_CHANGE, username, xuid), dimensionId(dimension) {}
    int dimensionId;
};

class PlayerUpdate : public PlayerStatusEvent {
public:
    PlayerUpdate() : PlayerStatusEvent(HockEventType::EVENT_PLAYER_UPDATE) {}
    PlayerUpdate(const std::string &username, const std::string &xuid, float pitch, float yaw, float X, float Y, float Z, float headYaw) : PlayerStatusEvent(HockEventType::EVENT_PLAYER_UPDATE,
                                                                                                                                                             username, xuid),
                                                                                                                                           pitch(pitch), yaw(yaw),
                                                                                                                                           X(X), Y(Y), Z(Z), headYaw(headYaw) {}

    float pitch{};
    float yaw{};
    float X{};
    float Y{};
    float Z{};
    float headYaw{};

    void Serialize(Json::Value &root) override {
        PlayerStatusEvent::Serialize(root);
        root["pitch"] = pitch;
        root["yaw"] = yaw;
        root["X"] = X;
        root["Y"] = Y;
        root["Z"] = Z;
        root["headYaw"] = headYaw;
    }

    void Deserialize(Json::Value &root) override {
        PlayerStatusEvent::Deserialize(root);
        pitch = root.get("pitch", 0.f).asFloat();
        yaw = root.get("yaw", 0.f).asFloat();
        X = root.get("X", 0.f).asFloat();
        Y = root.get("Y", 0.f).asFloat();
        Z = root.get("Z", 0.f).asFloat();
        headYaw = root.get("headYaw", 0.f).asFloat();
    }
};