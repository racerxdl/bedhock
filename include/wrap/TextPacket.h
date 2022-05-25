//
// Created by lucas on 29/11/2021.
//
#pragma once


#include <hook.h>
#include <vector>

// class WrappedTextPacket {
//     char content[0xFF]{};
//     Hook *hook;
// public:
//     explicit WrappedTextPacket(Hook *);

//     ~WrappedTextPacket();

//     Packet *get();

//     void createRaw(std::string const &message);

//     void createChat(std::string const &sourceName, std::string const &message, std::string const &param2 = "", std::string const &param3 = "");

//     void createTranslatedChat(std::string const &sourceName, std::string const &message, std::string const &param2 = "", std::string const &param3 = "");

//     void createTranslated(std::string const &message, const std::vector<std::string> &params = {});

//     void createSystemMessage(std::string const &message);

//     void createWhisper(std::string const &sourceName, std::string const &message, std::string const &param2 = "", std::string const &param3 = "");

//     void createAnnouncement(std::string const &message, std::string const &param2 = "", std::string const &param3 = "");

//     void createTranslatedAnnouncement(std::string const &message, std::string const &param2 = "", std::string const &param3 = "");

//     void createJukeboxPopup(std::string const &message, std::vector<std::string> const &params = {});
// };