//
// Created by lucas on 29/11/2021.
//

#include <wrap/TextPacket.h>
#include <hook.h>

WrappedTextPacket::WrappedTextPacket(Hook *hook) {
    this->hook = hook;
    hook->o_TextPacket_TextPacket(this->content);
}

WrappedTextPacket::~WrappedTextPacket() {
    hook->o_TextPacket_destructor(this->content);
}

Packet *WrappedTextPacket::get() {
    return (Packet *) &this->content[0];
}

void WrappedTextPacket::createRaw(std::string const &message) {
    hook->o_TextPacket_createRaw(get(), message);
}

void WrappedTextPacket::createChat(std::string const &sourceName, std::string const &message, std::string const &param2, std::string const &param3) {
    hook->o_TextPacket_createChat(get(), sourceName, message, param2, param3);
}

void WrappedTextPacket::createTranslatedChat(std::string const &sourceName, std::string const &message, std::string const &param2, std::string const &param3) {
    hook->o_TextPacket_createTranslatedChat(get(), sourceName, message, param2, param3);
}

void WrappedTextPacket::createTranslated(std::string const &message, const std::vector<std::string> &params) {
    hook->o_TextPacket_createTranslated(get(), message, params);
}

void WrappedTextPacket::createSystemMessage(std::string const &message) {
    hook->o_TextPacket_createSystemMessage(get(), message);
}

void WrappedTextPacket::createWhisper(std::string const &sourceName, std::string const &message, std::string const &param2, std::string const &param3) {
    hook->o_TextPacket_createWhisper(get(), sourceName, message, param2, param3);
}

void WrappedTextPacket::createAnnouncement(std::string const &message, std::string const &param2, std::string const &param3) {
    hook->o_TextPacket_createAnnouncement(get(), "SERVER", message, param2, param3);
}

void WrappedTextPacket::createTranslatedAnnouncement(std::string const &param1, std::string const &param2, std::string const &param3) {
    hook->o_TextPacket_createTranslatedAnnouncement(get(), "SERVER", param1, param2, param3);
}

void WrappedTextPacket::createJukeboxPopup(std::string const &message, std::vector<std::string> const &params) {
    hook->o_TextPacket_createJukeboxPopup(get(), message, params);
}
