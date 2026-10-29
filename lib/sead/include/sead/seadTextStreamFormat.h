#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStreamFormat.h"

namespace sead {
// RTTI N4sead16TextStreamFormatE @ 0x008D1B08
// vtable 0x00905E48 (vptr 0x00905E50), offset_to_top 0, 29 entries
//
// Values as text: tokens between separator characters, numbers in decimal (or with a 0b / 0x / 0
// prefix), bits as "0b...", strings in quotes ('\"' escapes a quote), memory blocks as Base64 in
// quotes; '#' and '//' comment out the rest of the line, '/* */' a block. Shift-JIS double bytes are
// kept together. All tokens go through one buffer under one lock (both in seadTextStreamFormat.cpp).
// The member and helper names are ours.
class TextStreamFormat : public ::sead::StreamFormat
{
public:
    TextStreamFormat(); // 0x005489A0 (name is ours)
    // (not virtual: the format has no destructor slot)
    ~TextStreamFormat(); // 0x00548AC0 (name is ours)

    virtual u8 readU8(StreamSrc* src, Endian::Types endian); // 0x005477A4 slot 0x00
    virtual u16 readU16(StreamSrc* src, Endian::Types endian); // 0x00547D98 slot 0x04
    virtual u32 readU32(StreamSrc* src, Endian::Types endian); // 0x00547E24 slot 0x08
    virtual u64 readU64(StreamSrc* src, Endian::Types endian); // 0x00547EB0 slot 0x0C
    virtual s8 readS8(StreamSrc* src, Endian::Types endian); // 0x00547718 slot 0x10
    virtual s16 readS16(StreamSrc* src, Endian::Types endian); // 0x00547BE4 slot 0x14
    virtual s32 readS32(StreamSrc* src, Endian::Types endian); // 0x00547C70 slot 0x18
    virtual s64 readS64(StreamSrc* src, Endian::Types endian); // 0x00547CFC slot 0x1C
    virtual f32 readF32(StreamSrc* src, Endian::Types endian); // 0x00547B0C slot 0x20
    virtual void readBit(StreamSrc* src, void* dst, u32 bitNum); // 0x00547840 slot 0x24
    virtual void readString(StreamSrc* src, BufferedSafeStringBase<char>* dst, u32 size); // 0x005468E8 slot 0x28
    virtual u32 readMemBlock(StreamSrc* src, void* dst, u32 size); // 0x005473A8 slot 0x2C
    virtual void writeU8(StreamSrc* src, Endian::Types endian, u8 value); // 0x00548028 slot 0x30
    virtual void writeU16(StreamSrc* src, Endian::Types endian, u16 value); // 0x00548700 slot 0x34
    virtual void writeU32(StreamSrc* src, Endian::Types endian, u32 value); // 0x005487DC slot 0x38
    virtual void writeU64(StreamSrc* src, Endian::Types endian, u64 value); // 0x005488B8 slot 0x3C
    virtual void writeS8(StreamSrc* src, Endian::Types endian, s8 value); // 0x00547F4C slot 0x40
    virtual void writeS16(StreamSrc* src, Endian::Types endian, s16 value); // 0x00548460 slot 0x44
    virtual void writeS32(StreamSrc* src, Endian::Types endian, s32 value); // 0x0054853C slot 0x48
    virtual void writeS64(StreamSrc* src, Endian::Types endian, s64 value); // 0x00548618 slot 0x4C
    virtual void writeF32(StreamSrc* src, Endian::Types endian, f32 value); // 0x00548374 slot 0x50
    virtual void writeBit(StreamSrc* src, const void* data, u32 bitNum); // 0x00548104 slot 0x54
    virtual void writeString(StreamSrc* src, const SafeStringBase<char>& str, u32 size); // 0x005469B8 slot 0x58
    virtual void writeMemBlock(StreamSrc* src, const void* data, u32 size); // 0x00547480 slot 0x5C
    virtual void writeDecorationText(StreamSrc* src, const SafeStringBase<char>& text); // 0x0054765C slot 0x60
    virtual void writeNullChar(StreamSrc* src); // 0x00547634 slot 0x64
    virtual void skip(StreamSrc* src, u32 size); // 0x005476E0 slot 0x68
    virtual void flush(StreamSrc* src); // 0x00547714 slot 0x6C
    virtual void rewind(StreamSrc* src); // 0x00547830 slot 0x70

    // the next token of src into the token buffer (empty at the end of src)
    void getNextToken_(StreamSrc* src); // 0x00546C0C

    // writes a formatted number and a separator
    template <typename T>
    void writeNumber_(StreamSrc* src, const char* format, T value);
    // writes the first separator character
    void writeSeparator_(StreamSrc* src);

    // the characters between tokens; the first one is written after a value
    FixedSafeString<128> mSeparator; // 0x04
};
ASSERT_SIZE(TextStreamFormat, 0x90);
} // namespace sead
