#pragma once

#include "base.h"

#pragma pack(push, 4)

enum class PacketReliability {
    Relible,
    RelibleOrdered
};

struct Packet {
    unsigned unk2 = 2;
    PacketReliability reliableOrdered = PacketReliability::RelibleOrdered;
    unsigned char clientSubId = 0;
    uint64_t unk24 = 0;
    uint64_t unk40 = 0;
    uint32_t incompressible = 0;
    inline enum StreamReadResult read(class ReadOnlyBinaryStream& binaryStream) {
        return _read(binaryStream);
    }
    virtual ~Packet();
    virtual int getId() const = 0;
    virtual std::string getName() const = 0;
    virtual void write(class BinaryStream &) const = 0;
    virtual struct ExtendedStreamReadResult readExtended(class ReadOnlyBinaryStream &);
    virtual bool disallowBatching() const;
    virtual enum StreamReadResult _read(class ReadOnlyBinaryStream &) = 0;
};

#pragma pack(pop)
