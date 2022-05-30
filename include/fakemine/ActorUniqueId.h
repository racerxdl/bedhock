#pragma

#include "base.h"

struct ActorUniqueID {
    long long id;
    ActorUniqueID() {
        id = -1;
    }

    ActorUniqueID(long long i) {
        id = i;
    }

    inline long long get() const {
        return id;
    }

    inline operator long long() const {
        return id;
    }

    uint64_t getHash() const;
};