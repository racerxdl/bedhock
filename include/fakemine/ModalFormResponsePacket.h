#pragma once

#include <iostream>

#include "base.h"
#include "packet.h"

class ModalFormResponsePacket : public Packet {
public:
    int formId;
    std::string data;
    class ModalFormResponsePacket& operator=(class ModalFormResponsePacket const &) = delete;
    ModalFormResponsePacket(class ModalFormResponsePacket const &) = delete;

public:
    virtual ~ModalFormResponsePacket();
    virtual int getId() const;
    virtual std::string getName() const;
    virtual void write(class BinaryStream &) const;
    virtual enum StreamReadResult _read(class ReadOnlyBinaryStream &);
    ModalFormResponsePacket();
};