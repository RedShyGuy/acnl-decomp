#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyDerivation.h"

namespace nn {
namespace nex {
// KeyDerivation with MD5Checksum::DeriveKey: the key of an id takes m_BaseIterations + id %
// m_Modulus iterations (the one from another key id % m_Modulus). The member names are ours.
// RTTI N2nn3nex16MD5KeyDerivationE @ 0x008CE5C0
// vtable 0x008FD18C (vptr 0x008FD194), offset_to_top 0, 8 entries
class MD5KeyDerivation : public ::nn::nex::KeyDerivation
{
public:
    // (inline)
    MD5KeyDerivation(u32 baseIterations, u32 modulus) : m_BaseIterations(baseIterations), m_Modulus(modulus) {}
    virtual ~MD5KeyDerivation(); // 0x00382518 slot 0x00
    // 0x00382514 slot 0x04 (deleting dtor)
    virtual nn::nex::Key CreateKey(unsigned id, const char* pPassword); // 0x003824C0 slot 0x08 | mk7dlp:bytes-fuzzy
    virtual nn::nex::Key InitializeKey(const char* pPassword); // 0x003823C0 slot 0x0C (name is ours)
    virtual nn::nex::Key CreateKey(unsigned id, const nn::nex::Key& key); // 0x0038230C slot 0x10 (name is ours)
    virtual bool CreateKey(nn::nex::CallContext* pContext, unsigned int id, const char* pPassword, nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache); // 0x00382450 slot 0x14 | fefates:bytes
    virtual bool InitializeKey(nn::nex::CallContext* pContext, const char* pPassword, nn::nex::Key* pKey); // 0x00382400 slot 0x18 | fefates:bytes
    virtual bool CreateKey(nn::nex::CallContext* pContext, unsigned int id, const nn::nex::Key& key, nn::nex::Key* pKey); // 0x00382230 slot 0x1C (name is ours)

    u32 m_BaseIterations; // 0x04
    u32 m_Modulus;        // 0x08
};
ASSERT_SIZE(MD5KeyDerivation, 0x0C);
} // namespace nex
} // namespace nn
