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

void WrappedTextPacket::createRaw(const std::string &) {

}

void WrappedTextPacket::createChat(const std::string &param0, const std::string &param1, const std::string &param2, const std::string &param3) {
    hook->o_TextPacket_createChat((TextPacket *) get(), param0, param1, param2, param3);
}

void WrappedTextPacket::createTranslatedChat(const std::string &, const std::string &, const std::string &, const std::string &) {

}

void WrappedTextPacket::createTranslated(const std::string &, const std::vector<std::string> &) {

}

void WrappedTextPacket::createSystemMessage(const std::string &) {

}

void WrappedTextPacket::createWhisper(const std::string &, const std::string &, const std::string &, const std::string &) {

}

void WrappedTextPacket::createAnnouncement(const std::string &, const std::string &, const std::string &, const std::string &) {

}

void WrappedTextPacket::createTranslatedAnnouncement(const std::string &, const std::string &, const std::string &, const std::string &) {

}

void WrappedTextPacket::createJukeboxPopup(const std::string &) {

}
