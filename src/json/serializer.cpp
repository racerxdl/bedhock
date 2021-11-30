//
// Created by lucas on 29/11/2021.
//

#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>
#include "json/serializer.h"

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
