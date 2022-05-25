#pragma once

#include <event/message.h>
#include <log.h>

#include <iostream>

#include "base.h"
#include "packet.h"

class TextPacket : public Packet {
    char filler[184];

   public:
    class TextPacket &operator=(class TextPacket const &) = delete;
    TextPacket(class TextPacket const &);

    virtual ~TextPacket();
    virtual int getId() const;
    virtual std::string getName() const;
    virtual void write(class BinaryStream &) const;
    virtual enum StreamReadResult _read(class ReadOnlyBinaryStream &);

    TextPacket();
    static TextPacket createAnnouncement(std::string const &, std::string const & = "", std::string const & = "", std::string const & = "");
    static TextPacket createChat(std::string const &from, std::string const &message, std::string const &a = "", std::string const &b = "");
    static TextPacket createJukeboxPopup(std::string const &, std::vector<std::string> const & = {});
    static TextPacket createSystemMessage(std::string const &);
    static TextPacket createTextObjectMessage(class ResolvedTextObject const &, std::string, std::string);
    static TextPacket createTextObjectWhisperMessage(class ResolvedTextObject const &, std::string const &, std::string const &);
    static TextPacket createTranslated(std::string const &, std::vector<std::string> const &);
    static TextPacket createTranslatedChat(std::string const &sourceName, std::string const &message, std::string const &param2 = "", std::string const &param3 = "");
    static TextPacket createTranslatedAnnouncement(std::string const &, std::string const & = "", std::string const & = "", std::string const & = "");
    static TextPacket createWhisper(std::string const &, std::string const &, std::string const & = "", std::string const & = "");

    // Bedhock
    static TextPacket createFromEvent(const MessageEvent *event) {
        switch ((MessageType)event->msgType) {
            case MessageType::NORMAL:
                if (event->translatable) {
                    return TextPacket::createTranslatedChat(event->from, event->message);
                } else {
                    return TextPacket::createChat(event->from, event->message);
                }
                break;
            case MessageType::ANOUNCE:
                if (event->translatable) {
                    return TextPacket::createTranslatedAnnouncement("SERVER", event->message);
                } else {
                    return TextPacket::createAnnouncement("SERVER", event->message);
                }
                break;
            case MessageType::WHISPER:
                return TextPacket::createWhisper(event->from, event->message);
            case MessageType::SYSTEM:
                return TextPacket::createSystemMessage(event->message);
            case MessageType::JUKEBOX:
                return TextPacket::createJukeboxPopup(event->message);
            default:
                Log::Error("Unexpected message type: {}\n", (int)event->msgType);
                return TextPacket::createSystemMessage(fmt::format("unexpected message type: {}", (int)event->msgType));
        }
    }
};

static_assert(sizeof(TextPacket) == 0xE0);