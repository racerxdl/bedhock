#pragma once

#include "base.h"

class IdentityDefinition {
   public:
    enum class Type : char {
        Invalid = 0,
        Player = 1,
        Actor = 2,
        Fake = 3
    };

    class IdentityDefinition &operator=(class IdentityDefinition const &) = delete;
    IdentityDefinition(class IdentityDefinition const &) = delete;
    IdentityDefinition() = delete;

    struct ActorUniqueID const &getEntityId() const;
    std::string const &getFakePlayerName() const;
    enum IdentityDefinition::Type getIdentityType() const;
    std::string const &getName(class std::function<std::string const &(struct ActorUniqueID)> const &) const;
    struct PlayerScoreboardId const &getPlayerId() const;
    bool isEntityType() const;
    bool isPlayerType() const;
    bool isValid() const;
    ~IdentityDefinition();
    static class IdentityDefinition const Invalid;
};