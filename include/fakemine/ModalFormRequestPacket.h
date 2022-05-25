
#pragma once

#include <iostream>

#include "base.h"
#include "packet.h"

class ModalFormRequestPacket : public Packet {
   public:
    class ModalFormRequestPacket &operator=(class ModalFormRequestPacket const &) = delete;
    ModalFormRequestPacket(class ModalFormRequestPacket const &) = delete;

    virtual ~ModalFormRequestPacket();
    virtual int getId() const;
    virtual std::string getName() const;
    virtual void write(class BinaryStream &) const;
    virtual void read(ReadOnlyBinaryStream &);

    uint formId;
    std::string jsonData;
};
