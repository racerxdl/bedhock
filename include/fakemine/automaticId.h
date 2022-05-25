#pragma once

template <typename A, typename T>
class AutomaticID {
    T id;

   public:
    AutomaticID() {
        id = 0;
    }

    AutomaticID(T x) {
        id = x;
    }

    inline operator T() const {
        return id;
    }
};