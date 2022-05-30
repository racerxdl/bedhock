#pragma once

#include "IdentityDefinition.h"
#include "ScoreBoardId.h"
#include "base.h"
#include "ActorUniqueId.h"

struct ScorePacketInfo {
   public:
    ScoreboardId sid;
    std::string objectiveName;
    uint32_t score;
    IdentityDefinition::Type type;
    unsigned long long pid;
    ActorUniqueID aid;
    std::string fake_name;

    ScorePacketInfo(ScoreboardId s, std::string objectiveName, IdentityDefinition::Type type, unsigned num, const std::string& fake)
        : sid(s), objectiveName(objectiveName), type(type), score(num), fake_name(fake) {
    }

    ~ScorePacketInfo() {}
};