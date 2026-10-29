#include "pead/peadBinaryStreamFormat.h"
#include "pead/peadBufferedSafeStringBase.h"
#include "pead/peadStreamSrc.h"

#include <cstring>

namespace pead {
// 0x0053AC44 slot 0x00
u8 pead::BinaryStreamFormat::readU8(StreamSrc* src, Endian::Types endian)
{
    u8 value = 0;
    src->read(&value, sizeof(value));
    return Endian::toHostU8(endian, value);
}

// 0x0053AE84 slot 0x04
u16 pead::BinaryStreamFormat::readU16(StreamSrc* src, Endian::Types endian)
{
    u16 value = 0;
    src->read(&value, sizeof(value));
    return Endian::toHostU16(endian, value);
}

// 0x0053AED8 slot 0x08
u32 pead::BinaryStreamFormat::readU32(StreamSrc* src, Endian::Types endian)
{
    u32 value = 0;
    src->read(&value, sizeof(value));
    return Endian::toHostU32(endian, value);
}

// 0x0053AF2C slot 0x0C
u64 pead::BinaryStreamFormat::readU64(StreamSrc* src, Endian::Types endian)
{
    u64 value = 0;
    src->read(&value, sizeof(value));
    return Endian::toHostU64(endian, value);
}

// 0x0053ABF0 slot 0x10
s8 pead::BinaryStreamFormat::readS8(StreamSrc* src, Endian::Types endian)
{
    u8 value = 0;
    src->read(&value, sizeof(value));
    return static_cast<s8>(Endian::toHostU8(endian, value));
}

// 0x0053AD74 slot 0x14
s16 pead::BinaryStreamFormat::readS16(StreamSrc* src, Endian::Types endian)
{
    u16 value = 0;
    src->read(&value, sizeof(value));
    return static_cast<s16>(Endian::toHostU16(endian, value));
}

// 0x0053ADD0 slot 0x18
s32 pead::BinaryStreamFormat::readS32(StreamSrc* src, Endian::Types endian)
{
    u32 value = 0;
    src->read(&value, sizeof(value));
    return static_cast<s32>(Endian::toHostU32(endian, value));
}

// 0x0053AE24 slot 0x1C
s64 pead::BinaryStreamFormat::readS64(StreamSrc* src, Endian::Types endian)
{
    u64 value = 0;
    src->read(&value, sizeof(value));
    return static_cast<s64>(Endian::toHostU64(endian, value));
}

// 0x0053AD1C slot 0x20
f32 pead::BinaryStreamFormat::readF32(StreamSrc* src, Endian::Types endian)
{
    u32 value = 0;
    src->read(&value, sizeof(value));
    u32 bits = Endian::toHostU32(endian, value);
    f32 result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

// 0x0053ACA4 slot 0x24
void pead::BinaryStreamFormat::readBit(StreamSrc* src, void* dst, u32 bitNum)
{
    u8* bytes = static_cast<u8*>(dst);
    u32 byteNum = bitNum / 8;
    src->read(bytes, byteNum);
    u32 restBits = bitNum - byteNum * 8;
    if (restBits != 0) {
        u8 last;
        src->read(&last, 1);
        u8 keepMask = 0xFF << restBits;
        bytes[byteNum] &= keepMask;
        bytes[byteNum] |= last & ~keepMask;
    }
}

// 0x0053A9B8 slot 0x28
void pead::BinaryStreamFormat::readString(StreamSrc* src, BufferedSafeStringBase<char>* dst, u32 size)
{
    u32 rest = 0;
    if (static_cast<u32>(dst->mBufferSize) < size) {
        rest = size - dst->mBufferSize;
        size = dst->mBufferSize;
    }
    src->read(dst->getBuffer(), size);
    dst->trim(static_cast<u32>(dst->mBufferSize) > size + 1 ? size : dst->mBufferSize - 1);
    if (rest != 0) {
        // what does not fit is read into the buffer too (and dropped)
        src->read(dst->getBuffer(), rest);
    }
}

// 0x0053AB88 slot 0x2C
u32 pead::BinaryStreamFormat::readMemBlock(StreamSrc* src, void* dst, u32 size)
{
    return src->read(dst, size);
}

// 0x0053AFE0 slot 0x30
void pead::BinaryStreamFormat::writeU8(StreamSrc* src, Endian::Types endian, u8 value)
{
    value = Endian::fromHostU8(endian, value);
    src->write(&value, sizeof(value));
}

// 0x0053B1E4 slot 0x34
void pead::BinaryStreamFormat::writeU16(StreamSrc* src, Endian::Types endian, u16 value)
{
    value = Endian::fromHostU16(endian, value);
    src->write(&value, sizeof(value));
}

// 0x0053B238 slot 0x38
void pead::BinaryStreamFormat::writeU32(StreamSrc* src, Endian::Types endian, u32 value)
{
    value = Endian::fromHostU32(endian, value);
    src->write(&value, sizeof(value));
}

// 0x0053B28C slot 0x3C
void pead::BinaryStreamFormat::writeU64(StreamSrc* src, Endian::Types endian, u64 value)
{
    value = Endian::fromHostU64(endian, value);
    src->write(&value, sizeof(value));
}

// 0x0053AF8C slot 0x40
void pead::BinaryStreamFormat::writeS8(StreamSrc* src, Endian::Types endian, s8 value)
{
    u8 bytes = Endian::fromHostU8(endian, static_cast<u8>(value));
    src->write(&bytes, sizeof(bytes));
}

// 0x0053B0E8 slot 0x44
void pead::BinaryStreamFormat::writeS16(StreamSrc* src, Endian::Types endian, s16 value)
{
    u16 bytes = Endian::fromHostU16(endian, static_cast<u16>(value));
    src->write(&bytes, sizeof(bytes));
}

// 0x0053B140 slot 0x48
void pead::BinaryStreamFormat::writeS32(StreamSrc* src, Endian::Types endian, s32 value)
{
    u32 bytes = Endian::fromHostU32(endian, static_cast<u32>(value));
    src->write(&bytes, sizeof(bytes));
}

// 0x0053B194 slot 0x4C
void pead::BinaryStreamFormat::writeS64(StreamSrc* src, Endian::Types endian, s64 value)
{
    u64 bytes = Endian::fromHostU64(endian, static_cast<u64>(value));
    src->write(&bytes, sizeof(bytes));
}

// 0x0053B08C slot 0x50
void pead::BinaryStreamFormat::writeF32(StreamSrc* src, Endian::Types endian, f32 value)
{
    u32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    u32 bytes = Endian::fromHostU32(endian, bits);
    src->write(&bytes, sizeof(bytes));
}

// 0x0053B030 slot 0x54
void pead::BinaryStreamFormat::writeBit(StreamSrc* src, const void* data, u32 bitNum)
{
    const u8* bytes = static_cast<const u8*>(data);
    u8 byteNum = bitNum / 8;
    src->write(bytes, byteNum);
    if (bitNum - byteNum * 8 != 0) {
        src->write(bytes + byteNum, 1);
    }
}

// 0x0053AAC8 slot 0x58
void pead::BinaryStreamFormat::writeString(StreamSrc* src, const SafeStringBase<char>& str, u32 size)
{
    u32 length = str.getLength();
    if (length > size) {
        length = size;
    }
    src->write(str.getCString(), length);
    // filled up to size with null characters
    u8 zero = 0;
    for (u32 i = length; i < size; i++) {
        src->write(&zero, 1);
    }
}

// 0x0053ABAC slot 0x5C
void pead::BinaryStreamFormat::writeMemBlock(StreamSrc* src, const void* data, u32 size)
{
    src->write(data, size);
}

// 0x0053ABD4 slot 0x60
void pead::BinaryStreamFormat::writeDecorationText(StreamSrc*, const SafeStringBase<char>&)
{
    // empty (in the original too)
}

// 0x0053ABD0 slot 0x64
void pead::BinaryStreamFormat::writeNullChar(StreamSrc*)
{
    // empty (in the original too)
}

// 0x0053ABD8 slot 0x68
void pead::BinaryStreamFormat::skip(StreamSrc* src, u32 size)
{
    src->skip(size);
}

// 0x0053ABEC slot 0x6C
void pead::BinaryStreamFormat::flush(StreamSrc*)
{
    // empty (in the original too)
}

// 0x0053AC94 slot 0x70
void pead::BinaryStreamFormat::rewind(StreamSrc* src)
{
    src->rewind();
}

} // namespace pead
