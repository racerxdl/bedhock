//
// Created by lucas on 29/11/2021.
//

#pragma once

#include <jsoncpp/json/value.h>

class IJsonSerializable {
public:
    virtual ~IJsonSerializable() = default;

    virtual void Serialize(Json::Value &root) = 0;

    virtual void Deserialize(Json::Value &root) = 0;
};