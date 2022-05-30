#pragma once

#include "base.h"

struct ChatEvent {
public:
    struct ChatEvent& operator=(struct ChatEvent const &) = delete;
    ChatEvent() = delete;

    ChatEvent(struct ChatEvent &&);
    ChatEvent(struct ChatEvent const &);
    ~ChatEvent();

    std::string message;
};