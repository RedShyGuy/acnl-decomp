#include "pead/peadTextStreamFormat.h"
#include "pead/peadBase64.h"
#include "pead/peadMutex.h"
#include "pead/peadStreamSrc.h"
#include "pead/peadStringUtil.h"

#include <cstdio>

namespace pead {
namespace {
// the buffer of the current token (name is ours)
// 0x00AF82A8
FixedSafeString<1024> s_TextStreamTokenBuffer;
// the lock of s_TextStreamTokenBuffer (name is ours)
// 0x00AF86B4
Mutex s_TextStreamMutex;

// the first byte of a Shift-JIS double byte character
inline bool isSjisLeadByte(u8 c)
{
    return static_cast<u8>(c - 0x81) <= 0x1E || static_cast<u8>(c - 0xE0) <= 0xF;
}
} // namespace

// constructs s_TextStreamTokenBuffer and s_TextStreamMutex
// 0x0079DB68 (static initializer)

// 0x0053A8AC (name after sead)
pead::TextStreamFormat::TextStreamFormat() : mSeparator(SafeStringBase<char>(" \t\r\n"))
{
}

// 0x0053A9A8 (name after sead)
pead::TextStreamFormat::~TextStreamFormat()
{
    // (only the members are destroyed)
}

// 0x00538C94 (name after sead)
void pead::TextStreamFormat::getNextToken_(StreamSrc* src)
{
    FixedSafeString<1024>& token = s_TextStreamTokenBuffer;
    s32 length = 0;
    // the end of the comment that is skipped: '\n' (line), '*' (block, waiting for "*"), '/' (block,
    // waiting for "/" after "*")
    char commentEnd = 0;
    bool isQuoted = false;
    // the last character was the second byte of a Shift-JIS character
    bool isAfterDoubleByte = false;
    token.getMutableStringTop()[0] = SafeStringBase<char>::s_NullChar;
    while (true) {
        char c;
        if (src->read(&c, 1) == 0) {
            return;
        }
        if (commentEnd != 0) {
            if (isSjisLeadByte(c)) {
                if (src->read(&c, 1) == 0) {
                    return;
                }
                continue;
            }
            if (c != commentEnd) {
                if (commentEnd == '/') {
                    commentEnd = '*';
                }
                continue;
            }
            if (commentEnd == '*') {
                commentEnd = '/';
                continue;
            }
            // the end of the comment separates like a separator
            commentEnd = 0;
            c = mSeparator.at(0);
        }
        if (isQuoted) {
            if (c == '"') {
                if (isAfterDoubleByte || length <= 1 || token.at(length - 1) != '\\') {
                    return;
                }
                // \" is a quote in the string
                token.overwriteAt(length - 1, SafeStringBase<char>("\""), 1);
                continue;
            }
        } else {
            if (length == 0 && c == '"') {
                isQuoted = true;
                continue;
            }
            if (c == '\0' || mSeparator.include(c)) {
                if (token.getMutableStringTop()[0] != SafeStringBase<char>::s_NullChar) {
                    return;
                }
                continue;
            }
        }
        token.append(c);
        length++;
        if (isSjisLeadByte(c)) {
            if (src->read(&c, 1) == 0) {
                return;
            }
            token.append(c);
            isAfterDoubleByte = true;
            length++;
            continue;
        }
        isAfterDoubleByte = false;
        if (isQuoted) {
            continue;
        }
        if (length >= 1 && token.at(length - 1) == '#') {
            length -= 1;
            commentEnd = '\n';
            token.trim(length);
        }
        if (length >= 2 && token.at(length - 2) == '/') {
            if (token.at(length - 1) == '/') {
                length -= 2;
                commentEnd = '\n';
                token.trim(length);
            } else if (token.at(length - 1) == '*') {
                length -= 2;
                commentEnd = '*';
                token.trim(length);
            }
        }
    }
}

// 0x00539794 slot 0x00
u8 pead::TextStreamFormat::readU8(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    u8 value = 0;
    getNextToken_(src);
    StringUtil::tryParseU8(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539D40 slot 0x04
u16 pead::TextStreamFormat::readU16(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    u16 value = 0;
    getNextToken_(src);
    StringUtil::tryParseU16(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539DCC slot 0x08
u32 pead::TextStreamFormat::readU32(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    u32 value = 0;
    getNextToken_(src);
    StringUtil::tryParseU32(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539E58 slot 0x0C
u64 pead::TextStreamFormat::readU64(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    u64 value = 0;
    getNextToken_(src);
    StringUtil::tryParseU64(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539708 slot 0x10
s8 pead::TextStreamFormat::readS8(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    s8 value = 0;
    getNextToken_(src);
    StringUtil::tryParseS8(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539B8C slot 0x14
s16 pead::TextStreamFormat::readS16(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    s16 value = 0;
    getNextToken_(src);
    StringUtil::tryParseS16(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539C18 slot 0x18
s32 pead::TextStreamFormat::readS32(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    s32 value = 0;
    getNextToken_(src);
    StringUtil::tryParseS32(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539CA4 slot 0x1C
s64 pead::TextStreamFormat::readS64(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    s64 value = 0;
    getNextToken_(src);
    StringUtil::tryParseS64(&value, SafeStringBase<char>(s_TextStreamTokenBuffer.getCString()), StringUtil::BASE_AUTO);
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539AC0 slot 0x20
f32 pead::TextStreamFormat::readF32(StreamSrc* src, Endian::Types)
{
    s_TextStreamMutex.lock();
    f32 value = 0.0f;
    getNextToken_(src);
    if (s_TextStreamTokenBuffer.getLength() != 0) {
        std::sscanf(s_TextStreamTokenBuffer.getCString(), "%f", &value);
    }
    s_TextStreamMutex.unlock();
    return value;
}

// 0x00539830 slot 0x24
void pead::TextStreamFormat::readBit(StreamSrc* src, void* dst, u32 bitNum)
{
    s_TextStreamMutex.lock();
    getNextToken_(src);
    SafeStringBase<char> str(s_TextStreamTokenBuffer.getCString());
    if (str.isEqualHead(SafeStringBase<char>("0b"), 2)) {
        str = str.getPart(2);
    }
    u8* bytes = static_cast<u8*>(dst);
    u32 bitIndex = 0;
    u8 bits = 0;
    // the digits up to the null character included
    s32 end = str.getLength() + 1;
    for (s32 i = 0; i != end && bitIndex < bitNum; i++) {
        bits = static_cast<u8>(bits << 1);
        char c = str.at(i);
        bitIndex++;
        if (c == '1') {
            bits |= 1;
        }
        if (bitIndex % 8 == 0) {
            bytes[bitIndex / 8 - 1] = bits;
            bits = 0;
        }
    }
    if (bitIndex % 8 != 0) {
        // the other bits of the last byte are kept
        bytes[bitIndex / 8] = (bytes[bitIndex / 8] & (0xFF << (bitIndex % 8))) | bits;
    }
    s_TextStreamMutex.unlock();
}

// 0x00538998 slot 0x28
void pead::TextStreamFormat::readString(StreamSrc* src, BufferedSafeStringBase<char>* dst, u32)
{
    s_TextStreamMutex.lock();
    getNextToken_(src);
    dst->copy(s_TextStreamTokenBuffer);
    s_TextStreamMutex.unlock();
}

// 0x005393BC slot 0x2C
u32 pead::TextStreamFormat::readMemBlock(StreamSrc* src, void* dst, u32 size)
{
    s_TextStreamMutex.lock();
    getNextToken_(src);
    s32 length = s_TextStreamTokenBuffer.getLength();
    s32 decodedSize = 0;
    Base64::decode(dst, size, s_TextStreamTokenBuffer.getCString(), length, &decodedSize);
    s_TextStreamMutex.unlock();
    return decodedSize;
}

void pead::TextStreamFormat::writeSeparator_(StreamSrc* src)
{
    src->write(mSeparator.getCString(), 1);
}

template <typename T>
void pead::TextStreamFormat::writeNumber_(StreamSrc* src, const char* format, T value)
{
    FixedSafeString<32> str;
    str.format(format, value);
    s32 length = str.getLength();
    src->write(str.getCString(), length);
    writeSeparator_(src);
}

// 0x00539FC4 slot 0x30
void pead::TextStreamFormat::writeU8(StreamSrc* src, Endian::Types, u8 value)
{
    writeNumber_(src, "%u", value);
}

// 0x0053A630 slot 0x34
void pead::TextStreamFormat::writeU16(StreamSrc* src, Endian::Types, u16 value)
{
    writeNumber_(src, "%u", value);
}

// 0x0053A700 slot 0x38
void pead::TextStreamFormat::writeU32(StreamSrc* src, Endian::Types, u32 value)
{
    writeNumber_(src, "%u", value);
}

// 0x0053A7D0 slot 0x3C
void pead::TextStreamFormat::writeU64(StreamSrc* src, Endian::Types, u64 value)
{
    writeNumber_(src, "%llu", value);
}

// 0x00539EF4 slot 0x40
void pead::TextStreamFormat::writeS8(StreamSrc* src, Endian::Types, s8 value)
{
    writeNumber_(src, "%d", value);
}

// 0x0053A3B4 slot 0x44
void pead::TextStreamFormat::writeS16(StreamSrc* src, Endian::Types, s16 value)
{
    writeNumber_(src, "%d", value);
}

// 0x0053A484 slot 0x48
void pead::TextStreamFormat::writeS32(StreamSrc* src, Endian::Types, s32 value)
{
    writeNumber_(src, "%d", value);
}

// 0x0053A554 slot 0x4C
void pead::TextStreamFormat::writeS64(StreamSrc* src, Endian::Types, s64 value)
{
    writeNumber_(src, "%lld", value);
}

// 0x0053A2D4 slot 0x50
void pead::TextStreamFormat::writeF32(StreamSrc* src, Endian::Types, f32 value)
{
    writeNumber_(src, "%.8f", static_cast<f64>(value));
}

// 0x0053A094 slot 0x54
void pead::TextStreamFormat::writeBit(StreamSrc* src, const void* data, u32 bitNum)
{
    s_TextStreamMutex.lock();
    FixedSafeString<1024>& token = s_TextStreamTokenBuffer;
    token.copy(SafeStringBase<char>("0b"));
    const u8* bytes = static_cast<const u8*>(data);
    u32 byteNum = (bitNum + 7) / 8;
    for (u32 i = 0; i < byteNum; i++) {
        u32 rest = bitNum - i * 8;
        s32 topBit = rest > 8 ? 7 : rest - 1;
        for (s32 bit = topBit; bit >= 0; bit--) {
            token.append((bytes[i] & (1 << bit)) != 0 ? '1' : '0');
        }
    }
    src->write(token.getCString(), bitNum + 2);
    writeSeparator_(src);
    s_TextStreamMutex.unlock();
}

// 0x00538A50 slot 0x58
void pead::TextStreamFormat::writeString(StreamSrc* src, const SafeStringBase<char>& str, u32 size)
{
    u32 length = str.getLength();
    if (size > length) {
        size = length;
    }
    char quote = '"';
    char backslash = '\\';
    src->write(&quote, 1);
    for (u32 i = 0; i < size; i++) {
        if (str.at(i) == '"') {
            src->write(&backslash, 1);
        }
        src->write(str.getCString() + i, 1);
    }
    src->write(&quote, 1);
}

// 0x00539488 slot 0x5C
void pead::TextStreamFormat::writeMemBlock(StreamSrc* src, const void* data, u32 size)
{
    s_TextStreamMutex.lock();
    FixedSafeString<1024>& token = s_TextStreamTokenBuffer;
    token.getMutableStringTop()[0] = SafeStringBase<char>::s_NullChar;
    u32 encodedSize = (size / 3 + (size % 3 != 0 ? 1 : 0)) * 4;
    if (static_cast<u32>(token.mBufferSize) > encodedSize + 1) {
        s32 blockNum = static_cast<s32>(size) / 3 + (static_cast<s32>(size) % 3 != 0 ? 1 : 0);
        token.getBuffer()[blockNum * 4] = '\0';
        Base64::encode(token.getMutableStringTop(), data, size, false);
        s32 length = token.getLength();
        src->write("\"", 1);
        src->write(token.getCString(), length);
        src->write("\"", 1);
        writeSeparator_(src);
    }
    s_TextStreamMutex.unlock();
}

// 0x00539658 slot 0x60
void pead::TextStreamFormat::writeDecorationText(StreamSrc* src, const SafeStringBase<char>& text)
{
    s32 length = text.getLength();
    src->write(text.getCString(), length);
}

// 0x00539630 slot 0x64
void pead::TextStreamFormat::writeNullChar(StreamSrc* src)
{
    char c = '\0';
    src->write(&c, 1);
}

// 0x005396D0 slot 0x68
void pead::TextStreamFormat::skip(StreamSrc* src, u32)
{
    s_TextStreamMutex.lock();
    getNextToken_(src);
    s_TextStreamMutex.unlock();
}

// 0x00539704 slot 0x6C
void pead::TextStreamFormat::flush(StreamSrc*)
{
    // empty (in the original too)
}

// 0x00539820 slot 0x70
void pead::TextStreamFormat::rewind(StreamSrc* src)
{
    src->rewind();
}

} // namespace pead
