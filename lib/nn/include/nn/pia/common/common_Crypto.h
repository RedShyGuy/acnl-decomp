#pragma once

// nn::pia::common::Crypto - encryption of the packets (AES-128 per block). The namespace, Mode,
// Setting and the functions are from the fefates symbols; the enumerators and members are ours.

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
namespace Crypto {

enum Mode : u8
{
    MODE_NONE = 0,
    MODE_AES128 = 1,
};

struct Setting
{
    Mode m_Mode;          // 0x0
    const void* m_pKey;   // 0x4
    u32 m_KeySize;        // 0x8
};
ASSERT_SIZE(Setting, 0xC);

// 16 for AES, 0 without encryption
size_t GetBlockSize(nn::pia::common::Crypto::Mode mode); // 0x00428E00 | fefates:callgraph [tier C]
// size must be a multiple of the block size
nn::Result Decrypt(void* pDst, const void* pSrc, unsigned int size, const nn::pia::common::Crypto::Setting& setting); // 0x00428E10 | fefates:callgraph [tier C]
nn::Result Encrypt(void* pDst, const void* pSrc, unsigned int size, const nn::pia::common::Crypto::Setting& setting); // 0x00428F08 | fefates:callgraph [tier C]

} // namespace Crypto
} // namespace common
} // namespace pia
} // namespace nn
