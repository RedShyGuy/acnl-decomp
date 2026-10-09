#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto21ShaBlock512BitContextE @ 0x008D04C8
//
// The block buffer and the big endian length padding of SHA-1 and SHA-256 (the state follows in
// the derived class at 0x50). The member names are ours.
class ShaBlock512BitContext : public ::nn::crypto::HashContextBase
{
public:
    static const size_t BLOCK_SIZE = 64;

    virtual void Update(const void* pData, size_t size); // 0x001437E4 | nintendogs:bytes [tier A]
    // continues a hash of size bytes (a multiple of the block size) whose state is pState (big
    // endian words); introduced here
    virtual void InitializeWithState(const void* pState, u64 size) = 0; // slot 0x20 (name is ours)

    // the 0x80 byte, the zeros and the length in bits up to the end of a block
    void AddPadding(); // 0x0014375C | nintendogs:bytes [tier A]

    u8 m_Block[BLOCK_SIZE]; // 0x04
    u32 m_BlockUsed;        // 0x44, bytes in m_Block
    // the blocks processed (two words: Sha1Context is 0x64 bytes, so there is no u64 member)
    u32 m_BlockCountLow;  // 0x48
    u32 m_BlockCountHigh; // 0x4C
};
ASSERT_OFFSET(ShaBlock512BitContext, m_BlockCountHigh, 0x4C);
ASSERT_SIZE(ShaBlock512BitContext, 0x50);
} // namespace crypto
} // namespace nn
