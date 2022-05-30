#pragma once

#include "ScorePacket.h"
#include "base.h"

class SetScorePacket : public Packet {
   public:
    SetScorePacket &operator=(SetScorePacket const &) = delete;
    SetScorePacket(SetScorePacket const &) = delete;
    SetScorePacket() = delete;

    virtual ~SetScorePacket();
    virtual int getId() const;
    virtual std::string getName() const;
    virtual void write(BinaryStream &) const;
    virtual enum StreamReadResult _read(ReadOnlyBinaryStream &);

    // static SetScorePacket change(struct ScoreboardId const &, Objective const &);
    // static SetScorePacket change(std::vector<struct ScorePacketInfo>);
    // static SetScorePacket remove(struct ScoreboardId const &, Objective const &);

    // SetScorePacket(enum ScorePacketType, struct ScoreboardId const &, Objective const &);
    SetScorePacket(std::vector<ScorePacketInfo>);
};