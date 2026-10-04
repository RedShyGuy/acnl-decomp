#include "nn/pia/common/common_Hmac.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_HashContextBase.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
namespace {
const u32 INNER_PAD = 0x36363636;
const u32 OUTER_PAD = 0x5C5C5C5C;
} // namespace

// 0x00428BE0 | fefates:bytes [tier B]
void nn::pia::common::Hmac::Initialize(nn::pia::common::HashContextBase* pHashContext, const void* pKey, unsigned int keySize)
{
    if (pHashContext == nullptr || keySize == 0) {
        m_pHashContext = nullptr;
        return;
    }
    m_pHashContext = pHashContext;
    u32 key[BLOCK_SIZE / 4];
    if (keySize > BLOCK_SIZE) {
        // a longer key is hashed
        m_pHashContext->Initialize();
        m_pHashContext->Update(pKey, keySize);
        m_pHashContext->GetHash(key);
        keySize = m_pHashContext->GetHashSize();
    } else {
        nnnstdMemCpy(key, pKey, keySize);
    }
    if (static_cast<int>(BLOCK_SIZE - keySize) > 0) {
        memset(reinterpret_cast<u8*>(key) + keySize, 0, BLOCK_SIZE - keySize);
    }
    for (int i = 0; i < static_cast<int>(BLOCK_SIZE / 4); i++) {
        m_InnerPad[i] = key[i] ^ INNER_PAD;
        m_OuterPad[i] = key[i] ^ OUTER_PAD;
    }
}

// 0x00428CD4 | fefates:bytes [tier B]
void nn::pia::common::Hmac::Calc(void* pOutput, const void* pData, unsigned int size)
{
    if (m_pHashContext == nullptr) {
        return;
    }
    u8 innerHash[MAX_HASH_SIZE];
    m_pHashContext->Initialize();
    m_pHashContext->Update(m_InnerPad, BLOCK_SIZE);
    m_pHashContext->Update(pData, size);
    m_pHashContext->GetHash(innerHash);
    m_pHashContext->Initialize();
    m_pHashContext->Update(m_OuterPad, BLOCK_SIZE);
    m_pHashContext->Update(innerHash, m_pHashContext->GetHashSize());
    m_pHashContext->GetHash(pOutput);
}

// 0x00428DB8 | fefates:callgraph [tier C]
nn::pia::common::Hmac::Hmac() : m_pHashContext(nullptr)
{
}

} // namespace common
} // namespace pia
} // namespace nn
