//
// Created by lucas on 29/11/2021.
//

#pragma once
#include "serializable.h"

class CJsonSerializer {
public:
    static bool Serialize(IJsonSerializable *pObj, std::string &output);

    static bool Deserialize(IJsonSerializable *pObj, std::string &input);

private:
    CJsonSerializer(void) {};
};