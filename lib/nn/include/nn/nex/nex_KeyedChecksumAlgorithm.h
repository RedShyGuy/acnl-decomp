#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"
#include "nn/nex/nex_Key.h"

namespace nn {
namespace nex {
// A checksum with a key of a length in a range. The member names are ours.
// RTTI N2nn3nex22KeyedChecksumAlgorithmE @ 0x008CEC44
// vtable 0x008FE154 (vptr 0x008FE15C), offset_to_top 0, 9 entries
class KeyedChecksumAlgorithm : public ::nn::nex::ChecksumAlgorithm
{
public:
    // the range of the key lengths (names are ours)
    static const u32 MIN_KEY_LENGTH = 6;
    static const u32 MAX_KEY_LENGTH = 64;

    // (inline)
    KeyedChecksumAlgorithm() : m_MinKeyLength(MIN_KEY_LENGTH), m_MaxKeyLength(MAX_KEY_LENGTH) {}
    virtual ~KeyedChecksumAlgorithm(); // 0x0039DAF4 slot 0x00
    // 0x0039DAC0 slot 0x04 (deleting dtor)
    // (inline, with a copy out of line) the key has a length in the range
    virtual bool IsReady() const { u32 length = m_Key.GetLength(); return length >= m_MinKeyLength && length <= m_MaxKeyLength; } // 0x0072D088 slot 0x10 | fefates:bytes
    virtual void KeyHasChanged(); // 0x0039DA70 slot 0x20
    // false if the length of the key is out of the range
    bool SetKey(const nn::nex::Key& key); // 0x0039DA74 | fefates:bytes [tier B]

    Key m_Key;          // 0x0C
    u32 m_MinKeyLength; // 0x24
    u32 m_MaxKeyLength; // 0x28
};
ASSERT_OFFSET(KeyedChecksumAlgorithm, m_Key, 0x0C);
ASSERT_SIZE(KeyedChecksumAlgorithm, 0x2C);
} // namespace nex
} // namespace nn
