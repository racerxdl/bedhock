//
// Created by lucas on 29/11/2021.
//
#pragma once


#include <hook.h>

class WrappedTextPacket {
    char content[0xE0];
    Hook *hook;
public:
    WrappedTextPacket(Hook *);
    ~WrappedTextPacket();
    Packet *get();

    void createRaw(std::string const&);
    void createChat(std::string const&, std::string const&, std::string const&, std::string const&);
    void createTranslatedChat(std::string const&, std::string const&, std::string const&, std::string const&);
    void createTranslated(std::string const&, std::vector<std::string> const&);
    void createSystemMessage(std::string const&);
    void createWhisper(std::string const&, std::string const&, std::string const&, std::string const&);
    void createAnnouncement(std::string const&, std::string const&, std::string const&, std::string const&);
    void createTranslatedAnnouncement(std::string const&, std::string const&, std::string const&, std::string const&);
    void createJukeboxPopup(std::string const&);
};