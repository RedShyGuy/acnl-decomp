#pragma once

#include "decomp.h"
#include "pead/peadFixedSafeString.h"
#include "pead/peadStreamFormat.h"

namespace pead {
// RTTI N4pead16TextStreamFormatE @ 0x008D1204
// vtable 0x00904B20 (vptr 0x00904B28), offset_to_top 0, 29 entries
//
// Values as text: tokens between separator characters, numbers in decimal (or with a 0b / 0x / 0
// prefix), bits as "0b...", strings in quotes ('\"' escapes a quote), memory blocks as Base64 in
// quotes; '#' and '//' comment out the rest of the line, '/* */' a block. Shift-JIS double bytes are
// kept together. All tokens go through one buffer under one lock (both in seadTextStreamFormat.cpp).
// The member and helper names are ours.
class TextStreamFormat : public ::pead::StreamFormat
{
public:
    TextStreamFormat(); // 0x0053A8AC (name after sead)
    // (not virtual: the format has no destructor slot)
    ~TextStreamFormat(); // 0x0053A9A8 (name after sead)

    virtual u8 readU8(StreamSrc* src, Endian::Types endian); // 0x00539794 slot 0x00
    virtual u16 readU16(StreamSrc* src, Endian::Types endian); // 0x00539D40 slot 0x04
    virtual u32 readU32(StreamSrc* src, Endian::Types endian); // 0x00539DCC slot 0x08
    virtual u64 readU64(StreamSrc* src, Endian::Types endian); // 0x00539E58 slot 0x0C
    virtual s8 readS8(StreamSrc* src, Endian::Types endian); // 0x00539708 slot 0x10
    virtual s16 readS16(StreamSrc* src, Endian::Types endian); // 0x00539B8C slot 0x14
    virtual s32 readS32(StreamSrc* src, Endian::Types endian); // 0x00539C18 slot 0x18
    virtual s64 readS64(StreamSrc* src, Endian::Types endian); // 0x00539CA4 slot 0x1C
    virtual f32 readF32(StreamSrc* src, Endian::Types endian); // 0x00539AC0 slot 0x20
    virtual void readBit(StreamSrc* src, void* dst, u32 bitNum); // 0x00539830 slot 0x24
    virtual void readString(StreamSrc* src, BufferedSafeStringBase<char>* dst, u32 size); // 0x00538998 slot 0x28
    virtual u32 readMemBlock(StreamSrc* src, void* dst, u32 size); // 0x005393BC slot 0x2C
    virtual void writeU8(StreamSrc* src, Endian::Types endian, u8 value); // 0x00539FC4 slot 0x30
    virtual void writeU16(StreamSrc* src, Endian::Types endian, u16 value); // 0x0053A630 slot 0x34
    virtual void writeU32(StreamSrc* src, Endian::Types endian, u32 value); // 0x0053A700 slot 0x38
    virtual void writeU64(StreamSrc* src, Endian::Types endian, u64 value); // 0x0053A7D0 slot 0x3C
    virtual void writeS8(StreamSrc* src, Endian::Types endian, s8 value); // 0x00539EF4 slot 0x40
    virtual void writeS16(StreamSrc* src, Endian::Types endian, s16 value); // 0x0053A3B4 slot 0x44
    virtual void writeS32(StreamSrc* src, Endian::Types endian, s32 value); // 0x0053A484 slot 0x48
    virtual void writeS64(StreamSrc* src, Endian::Types endian, s64 value); // 0x0053A554 slot 0x4C
    virtual void writeF32(StreamSrc* src, Endian::Types endian, f32 value); // 0x0053A2D4 slot 0x50
    virtual void writeBit(StreamSrc* src, const void* data, u32 bitNum); // 0x0053A094 slot 0x54
    virtual void writeString(StreamSrc* src, const SafeStringBase<char>& str, u32 size); // 0x00538A50 slot 0x58
    virtual void writeMemBlock(StreamSrc* src, const void* data, u32 size); // 0x00539488 slot 0x5C
    virtual void writeDecorationText(StreamSrc* src, const SafeStringBase<char>& text); // 0x00539658 slot 0x60
    virtual void writeNullChar(StreamSrc* src); // 0x00539630 slot 0x64
    virtual void skip(StreamSrc* src, u32 size); // 0x005396D0 slot 0x68
    virtual void flush(StreamSrc* src); // 0x00539704 slot 0x6C
    virtual void rewind(StreamSrc* src); // 0x00539820 slot 0x70

    // the next token of src into the token buffer (empty at the end of src)
    void getNextToken_(StreamSrc* src); // 0x00538C94 (name after sead)

    // writes a formatted number and a separator
    template <typename T>
    void writeNumber_(StreamSrc* src, const char* format, T value);
    // writes the first separator character
    void writeSeparator_(StreamSrc* src);

    // the characters between tokens; the first one is written after a value
    FixedSafeString<128> mSeparator; // 0x04
};
ASSERT_SIZE(TextStreamFormat, 0x90);
} // namespace pead
