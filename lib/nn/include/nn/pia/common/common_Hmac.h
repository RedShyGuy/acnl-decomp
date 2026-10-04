#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
class HashContextBase;

// HMAC (RFC 2104) with a hash context. Layout from Initialize / Calc; the member names are ours.
class Hmac : public RootObject
{
public:
    static const unsigned int BLOCK_SIZE = 64;
    static const unsigned int MAX_HASH_SIZE = 32;

    Hmac(); // 0x00428DB8 | fefates:callgraph [tier C]

    // sets the key; without a context or key the Hmac is off (Calc does nothing)
    void Initialize(nn::pia::common::HashContextBase* pHashContext, const void* pKey, unsigned int keySize); // 0x00428BE0 | fefates:bytes [tier B]
    void Calc(void* pOutput, const void* pData, unsigned int size); // 0x00428CD4 | fefates:bytes [tier B]

    HashContextBase* m_pHashContext; // 0x00
    u32 m_InnerPad[BLOCK_SIZE / 4];  // 0x04, key ^ 0x36
    u32 m_OuterPad[BLOCK_SIZE / 4];  // 0x44, key ^ 0x5C
};
ASSERT_SIZE(Hmac, 0x84);
} // namespace common
} // namespace pia
} // namespace nn
