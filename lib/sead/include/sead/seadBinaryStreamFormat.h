#pragma once

#include "decomp.h"
#include "sead/seadStreamFormat.h"

namespace sead {
// RTTI N4sead18BinaryStreamFormatE @ 0x008D1C38
// vtable 0x009060A8 (vptr 0x009060B0), offset_to_top 0, 29 entries
//
// Values as their bytes in the given byte order.
class BinaryStreamFormat : public ::sead::StreamFormat
{
public:
    virtual u8 readU8(StreamSrc* src, Endian::Types endian); // 0x00549934 slot 0x00
    virtual u16 readU16(StreamSrc* src, Endian::Types endian); // 0x00549B74 slot 0x04
    virtual u32 readU32(StreamSrc* src, Endian::Types endian); // 0x00549BC8 slot 0x08
    virtual u64 readU64(StreamSrc* src, Endian::Types endian); // 0x00549C1C slot 0x0C
    virtual s8 readS8(StreamSrc* src, Endian::Types endian); // 0x005498E0 slot 0x10
    virtual s16 readS16(StreamSrc* src, Endian::Types endian); // 0x00549A64 slot 0x14
    virtual s32 readS32(StreamSrc* src, Endian::Types endian); // 0x00549AC0 slot 0x18
    virtual s64 readS64(StreamSrc* src, Endian::Types endian); // 0x00549B14 slot 0x1C
    virtual f32 readF32(StreamSrc* src, Endian::Types endian); // 0x00549A0C slot 0x20
    virtual void readBit(StreamSrc* src, void* dst, u32 bitNum); // 0x00549994 slot 0x24
    virtual void readString(StreamSrc* src, BufferedSafeStringBase<char>* dst, u32 size); // 0x0054968C slot 0x28
    virtual u32 readMemBlock(StreamSrc* src, void* dst, u32 size); // 0x00549878 slot 0x2C
    virtual void writeU8(StreamSrc* src, Endian::Types endian, u8 value); // 0x00549CD0 slot 0x30
    virtual void writeU16(StreamSrc* src, Endian::Types endian, u16 value); // 0x00549ED4 slot 0x34
    virtual void writeU32(StreamSrc* src, Endian::Types endian, u32 value); // 0x00549F28 slot 0x38
    virtual void writeU64(StreamSrc* src, Endian::Types endian, u64 value); // 0x00549F7C slot 0x3C
    virtual void writeS8(StreamSrc* src, Endian::Types endian, s8 value); // 0x00549C7C slot 0x40
    virtual void writeS16(StreamSrc* src, Endian::Types endian, s16 value); // 0x00549DD8 slot 0x44
    virtual void writeS32(StreamSrc* src, Endian::Types endian, s32 value); // 0x00549E30 slot 0x48
    virtual void writeS64(StreamSrc* src, Endian::Types endian, s64 value); // 0x00549E84 slot 0x4C
    virtual void writeF32(StreamSrc* src, Endian::Types endian, f32 value); // 0x00549D7C slot 0x50
    virtual void writeBit(StreamSrc* src, const void* data, u32 bitNum); // 0x00549D20 slot 0x54
    virtual void writeString(StreamSrc* src, const SafeStringBase<char>& str, u32 size); // 0x005497AC slot 0x58
    virtual void writeMemBlock(StreamSrc* src, const void* data, u32 size); // 0x0054989C slot 0x5C
    virtual void writeDecorationText(StreamSrc* src, const SafeStringBase<char>& text); // 0x005498C4 slot 0x60
    virtual void writeNullChar(StreamSrc* src); // 0x005498C0 slot 0x64
    virtual void skip(StreamSrc* src, u32 size); // 0x005498C8 slot 0x68
    virtual void flush(StreamSrc* src); // 0x005498DC slot 0x6C
    virtual void rewind(StreamSrc* src); // 0x00549984 slot 0x70
};
} // namespace sead
