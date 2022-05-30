#pragma once

#include "base.h"

class SetDisplayObjectivePacket : public Packet {
public:

    class SetDisplayObjectivePacket& operator=(class SetDisplayObjectivePacket const &) = delete;
    SetDisplayObjectivePacket(class SetDisplayObjectivePacket const &) = delete;

    virtual ~SetDisplayObjectivePacket();
    virtual int getId() const;
    virtual std::string getName() const;
    virtual void write(class BinaryStream &) const;
    virtual enum StreamReadResult _read(class ReadOnlyBinaryStream &);
    SetDisplayObjectivePacket(std::string const &, std::string const &, std::string const &, std::string const &, ObjectiveSortOrder);
    SetDisplayObjectivePacket() = delete;
    char b[40];
    std::string DisplaySlot;
    std::string ObjectiveName;
    std::string DisplayName;
    std::string CriteriaName;
    ObjectiveSortOrder sortOrder;
    // std::string param4;
};