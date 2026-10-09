#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto10Md5ContextE @ 0x008D047C
// vtable 0x0090220C (vptr 0x00902214), offset_to_top 0, 9 entries
//
// MD5 (RFC 1321), the same code as nn::pia::common::Md5Context. The member names are ours.
class Md5Context : public ::nn::crypto::HashContextBase
{
public:
    static const size_t HASH_SIZE = 16;
    static const size_t BLOCK_SIZE = 64;

    Md5Context() {}

    virtual void Initialize(); // 0x00482820 slot 0x00 | fefates:callgraph
    virtual void Finalize(); // 0x00482DEC slot 0x04 (name is ours)
    virtual void Update(const void* pData, size_t size); // 0x00482B9C slot 0x08 | fefates:bytes
    virtual size_t GetHashSize() const; // 0x00482850 slot 0x0C (name is ours)
    virtual void GetHash(void* pOutput); // 0x00482C68 slot 0x10 | fefates:bytes
    virtual ~Md5Context(); // slots 0x14, 0x18
    // hashes m_Block into m_State
    virtual void ProcessBlock(); // 0x00482858 slot 0x1C | fefates:bytes
    // introduced here; returns GetHashSize()
    virtual size_t GetOutputSize() const; // 0x00482B90 slot 0x20 (name is ours)

    u32 m_State[4];         // 0x04
    u32 m_Size;             // 0x14, bytes so far
    u8 m_Block[BLOCK_SIZE]; // 0x18
};
ASSERT_SIZE(Md5Context, 0x58);
} // namespace crypto
} // namespace nn
