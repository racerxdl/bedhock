//
// Created by lucas on 29/11/2021.
//

#pragma once

#include <memory>
#include <event/event.h>
#include "serializable.h"

class CJsonSerializer {
public:
    static bool Serialize(IJsonSerializable *pObj, std::string &output);

    static bool Deserialize(IJsonSerializable *pObj, std::string &input);

    static std::shared_ptr<HockEvent> DeserializeEvent(const std::string &input);
private:
    CJsonSerializer() = default;
};