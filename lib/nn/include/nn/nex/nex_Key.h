#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// A key: its bytes (as a string in lowercase hexadecimal). The member names are ours.
// RTTI N2nn3nex3KeyE @ 0x008CF464
// vtable 0x008FF894 (vptr 0x008FF89C), offset_to_top 0, 2 entries
class Key : public ::nn::nex::RefCountedObject
{
public:
    Key(); // 0x003CCA2C | fefates:bytes [tier B]
    Key(const unsigned char* pData, unsigned size); // 0x003CC840 | mk7dlp:callseq-callee [tier A]
    // from the hexadecimal string (two digits per byte, lowercase)
    Key(const nn::nex::String& string); // 0x003CC8A4 | fefates:bytes [tier B]
    Key(const nn::nex::Key& other); // 0x003CC95C | fefates:bytes [tier B]
    virtual ~Key(); // 0x003CCA8C slot 0x00 | fefates:bytes
    // 0x003CCA5C slot 0x04 (deleting dtor)

    // the key gets size zero bytes; its bytes
    unsigned char* PrepareContentPtr(unsigned int size); // 0x003CC824 | fefates:bytes [tier B]
    nn::nex::Key& operator=(const nn::nex::Key& other); // 0x003CCB14 | fefates:bytes [tier B]
    // (inline, with copies out of line) the bytes, null if the key is empty
    const unsigned char* GetContentPtr() const { return m_Data.m_pBegin != m_Data.m_pEnd ? m_Data.m_pBegin : nullptr; } // 0x0072DF34 | fefates:bytes [tier B]
    u32 GetLength() const { return m_Data.m_pEnd - m_Data.m_pBegin; } // 0x0072E078 | fefates:bytes [tier C]
    // the bytes in hexadecimal
    void ExtractToString(nn::nex::String* pString) const; // 0x0072DF4C | fefates:bytes [tier B]
    nn::nex::String ToString() const; // 0x0072E040 | fefates:bytes [tier B]

    qVector<u8> m_Data; // 0x0C
};
ASSERT_OFFSET(Key, m_Data, 0x0C);
ASSERT_SIZE(Key, 0x18);
} // namespace nex
} // namespace nn
