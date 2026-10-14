#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_Md5Context.h"

namespace nn {
namespace nex {
// An MD5 hash over nn::crypto::Md5Context (all inline). The class name is from the reference symbols,
// the function and member names are ours.
class MD5
{
public:
    static const u32 DIGEST_SIZE = 16;

    MD5() : m_IsFinished(false) { m_Context.Initialize(); }

    // (nothing after the digest)
    void Update(const void* pData, u32 size)
    {
        if (!m_IsFinished) {
            m_Context.Update(pData, size);
        }
    }
    const u8* GetDigest()
    {
        if (!m_IsFinished) {
            m_Context.GetHash(m_Digest);
            m_IsFinished = true;
        }
        return m_Digest;
    }

    nn::crypto::Md5Context m_Context; // 0x00
    u8 m_Digest[DIGEST_SIZE];         // 0x58
    bool m_IsFinished;                // 0x68
};
ASSERT_OFFSET(MD5, m_Digest, 0x58);
ASSERT_SIZE(MD5, 0x6C);
} // namespace nex
} // namespace nn
