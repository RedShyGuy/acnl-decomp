#pragma once

#include "decomp.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_PluginObject.h"

namespace nn {
namespace nex {
class Buffer;

// An encryption with a key of a length in a range. The functions return 0 on success. The member
// names and the names of the functions without a reference name are ours.
// RTTI N2nn3nex19EncryptionAlgorithmE @ 0x008CE7D8
// vtable 0x008FD6A0 (vptr 0x008FD6A8), offset_to_top 0, 8 entries
class EncryptionAlgorithm : public ::nn::nex::PluginObject
{
public:
    // (inline)
    EncryptionAlgorithm() {}
    virtual ~EncryptionAlgorithm(); // 0x00393320 slot 0x00
    // 0x003932EC slot 0x04 | fefates:bytes (deleting dtor)
    virtual int Encrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput) = 0; // slot 0x08
    // (in place)
    virtual int Encrypt(nn::nex::Buffer* pBuffer); // 0x00393294 slot 0x0C | fefates:bytes
    virtual int Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput) = 0; // slot 0x10
    // (in place; 3 without a buffer)
    virtual int Decrypt(nn::nex::Buffer* pBuffer); // 0x00393230 slot 0x14 | fefates:bytes
    // the text of an error (false if it does not fit)
    virtual bool GetErrorString(int error, wchar_t* pString, u32 size); // 0x0039318C slot 0x18 (name is ours)
    virtual void KeyHasChanged(); // 0x00393188 slot 0x1C | slot vf_0x1C of nn::nex::EncryptionAlgorithm
    // false if the length of the key is out of the range
    bool SetKey(const nn::nex::Key& key); // 0x003931E4 | fefates:bytes [tier B]

    Key m_Key;            // 0x08
    u32 m_MinKeyLength;   // 0x20
    u32 m_MaxKeyLength;   // 0x24
};
ASSERT_OFFSET(EncryptionAlgorithm, m_Key, 0x08);
ASSERT_SIZE(EncryptionAlgorithm, 0x28);
} // namespace nex
} // namespace nn
