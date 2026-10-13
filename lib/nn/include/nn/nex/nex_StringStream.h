#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class String;
class StationURL;

// Text output into a growing buffer (first the buffer in the object). Numbers are written decimal
// or (after hex) hexadecimal with an optional "0x" and zero padding. The names of the members and
// of the functions without a reference name are ours.
// RTTI N2nn3nex12StringStreamE @ 0x008CE1D0
// vtable 0x008FC718 (vptr 0x008FC720), offset_to_top 0, 2 entries
class StringStream : public ::nn::nex::RootObject
{
public:
    static const u32 INLINE_LENGTH = 256;
    // the room for a number
    static const u32 NUMBER_LENGTH = 32;

    StringStream(); // 0x00361408 | mk7dlp:bytes [tier A]
    virtual ~StringStream(); // 0x00361478 slot 0x00 | fefates:bytes
    // 0x00361448 slot 0x04 (deleting dtor)

    nn::nex::StringStream& StreamNumber(unsigned char value); // 0x00361144 | fefates:bytes [tier B]
    nn::nex::StringStream& StreamNumber(int value); // 0x003611E4 | fefates:bytes [tier B]
    nn::nex::StringStream& StreamNumber(unsigned int value); // 0x00361284 | fefates:bytes [tier B]
    nn::nex::StringStream& StreamNumber(unsigned long long value); // 0x003615B8 (name is ours)
    // grows the buffer (by halves) so that count more characters fit
    void TestFreeRoom(unsigned int count); // 0x00361314 | mk7dlp:bytes-fuzzy [tier A]
    void Clear(); // 0x003613B4 | fefates:bytes [tier B]
    nn::nex::StringStream& operator<<(const void* p); // 0x003614C4 | mk7dlp:bytes-fuzzy [tier A]
    nn::nex::StringStream& operator<<(const wchar_t* pString); // 0x00361568 | fefates:bytes [tier B]
    // (inline)
    nn::nex::StringStream& operator<<(int value) { return StreamNumber(value); }
    nn::nex::StringStream& operator<<(unsigned int value) { return StreamNumber(value); }

    // the number of characters (inline in the original, with a copy out of line)
    u32 GetLength() const { return m_pCursor - m_pBuffer; } // 0x0072A604 | fefates:bytes [tier B]
    const wchar_t* CStr() const { return m_pBuffer; }

    wchar_t* m_pBuffer;                     // 0x004
    u32 m_Capacity;                         // 0x008 (characters)
    wchar_t* m_pCursor;                     // 0x00C (the terminating null)
    wchar_t m_InlineBuffer[INLINE_LENGTH];  // 0x010
    bool m_IsHex;                           // 0x210
    bool m_ShowsBase;                       // 0x211 ("0x" in front of hexadecimal numbers)
    bool m_Unknown0x212;                    // 0x212
    bool m_IsZeroPadded;                    // 0x213 (not set by the constructor)
};
ASSERT_OFFSET(StringStream, m_IsHex, 0x210);
ASSERT_SIZE(StringStream, 0x214);

nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::StationURL& url); // 0x0036153C | mk7dlp:bytes [tier A]
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::String& string); // 0x003DA0A0 | fefates:callgraph [tier C]
} // namespace nex
} // namespace nn
