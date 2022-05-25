#pragma once

#include "base.h"

class ReadOnlyBinaryStream {
    // Add Member There
   public:
    size_t readPointer{};
    bool unk;
    std::string ownBuf, *pBuf;

   public:
    std::string const &getData() const;
    size_t getLength() const;
    size_t getReadPointer() const;
    size_t getUnreadLength() const;
    void setReadPointer(std::size_t size);

    std::unique_ptr<class CompoundTag> getCompoundTag();

   public:
    class ReadOnlyBinaryStream &operator=(class ReadOnlyBinaryStream const &) = delete;
    ReadOnlyBinaryStream(class ReadOnlyBinaryStream const &) = delete;
    ReadOnlyBinaryStream() = delete;

   public:
    virtual ~ReadOnlyBinaryStream();
    virtual bool read(void *, uint64_t);

    ReadOnlyBinaryStream(std::string &&);
    ReadOnlyBinaryStream(std::string const &, bool);
    bool canReadBool() const;
    bool getBool();
    unsigned char getByte();
    double getDouble();
    float getFloat();
    enum StreamReadResult getReadCompleteResult() const;
    int getSignedBigEndianInt();
    int getSignedInt();
    int64_t getSignedInt64();
    short getSignedShort();
    std::string getString();
    bool getString(std::string &);
    unsigned char getUnsignedChar();
    unsigned int getUnsignedInt();
    uint64_t getUnsignedInt64();
    unsigned short getUnsignedShort();
    unsigned int getUnsignedVarInt();
    uint64_t getUnsignedVarInt64();
    int getVarInt();
    int64_t getVarInt64();
};

class BinaryStream : public ReadOnlyBinaryStream {
   public:
    std::string writeBuf, *pwBuf;

    void reserve(size_t size);
    std::string &getRaw();
    void writeCompoundTag(class CompoundTag const &tag);

    class BinaryStream &operator=(class BinaryStream const &) = delete;
    BinaryStream(class BinaryStream const &) = delete;
    virtual ~BinaryStream();

    BinaryStream(std::string &, bool);
    BinaryStream();
    std::string getAndReleaseData();
    void reset();
    void writeBool(bool);
    void writeByte(unsigned char);
    void writeDouble(double);
    void writeFloat(float);
    void writeSignedBigEndianInt(int);
    void writeSignedInt(int);
    void writeSignedInt64(int64_t);
    void writeSignedShort(short);
    void writeString(gsl::basic_string_span<char const, -1l>);
    void writeUnsignedChar(unsigned char);
    void writeUnsignedInt(unsigned int);
    void writeUnsignedInt64(uint64_t);
    void writeUnsignedShort(unsigned short);
    void writeUnsignedVarInt(unsigned int);
    void writeUnsignedVarInt64(uint64_t);
    void writeVarInt(int);
    void writeVarInt64(int64_t);
    void write(void const *, uint64_t);
};