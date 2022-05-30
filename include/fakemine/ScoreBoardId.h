#pragma once

#include "base.h"

struct ScoreboardId {
   public:
    int64_t id;
    void *null;
    ScoreboardId(struct ScoreboardId const &);
    ScoreboardId();
    ScoreboardId(int64_t);
    uint64_t getHash() const;
    class IdentityDefinition const &getIdentityDef() const;
    bool isValid() const;
    bool operator!=(struct ScoreboardId const &) const;
    struct ScoreboardId &operator=(struct ScoreboardId const &);
    bool operator==(struct ScoreboardId const &) const;
};