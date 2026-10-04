#include "nn/pia/common/common_Crypto.h"
#include "nn/crypto/crypto_Aes.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
namespace Crypto {
namespace {
const size_t AES_BLOCK_SIZE = 16;

// the key of the cipher (inline in Decrypt / Encrypt)
inline nn::Result SetKey(nn::crypto::BlockCipher& cipher, const Setting& setting)
{
    if (setting.m_Mode == MODE_AES128) {
        if (cipher.GetKeySize() != setting.m_KeySize) {
            return RESULT_INVALID_ARGUMENT;
        }
        cipher.SetKey(setting.m_pKey, setting.m_KeySize);
    }
    return nn::Result();
}
} // namespace

// 0x00428E00 | fefates:callgraph [tier C]
size_t GetBlockSize(nn::pia::common::Crypto::Mode mode)
{
    return mode == MODE_AES128 ? AES_BLOCK_SIZE : 0;
}

// 0x00428E10 | fefates:callgraph [tier C]
nn::Result Decrypt(void* pDst, const void* pSrc, unsigned int size, const nn::pia::common::Crypto::Setting& setting)
{
    switch (setting.m_Mode) {
    case MODE_NONE:
        return nn::Result();
    case MODE_AES128: {
        nn::crypto::Aes<16> aes;
        nn::Result result = SetKey(aes, setting);
        if (result.IsFailure()) {
            return result;
        }
        if (size % AES_BLOCK_SIZE != 0) {
            return RESULT_INVALID_ARGUMENT;
        }
        u8* pDstBlock = static_cast<u8*>(pDst);
        const u8* pSrcBlock = static_cast<const u8*>(pSrc);
        for (u32 i = 0; i < size / AES_BLOCK_SIZE; i++) {
            aes.Decrypt(pDstBlock, pSrcBlock);
            pDstBlock += AES_BLOCK_SIZE;
            pSrcBlock += AES_BLOCK_SIZE;
        }
        return nn::Result();
    }
    default:
        return RESULT_INVALID_ARGUMENT;
    }
}

// 0x00428F08 | fefates:callgraph [tier C]
nn::Result Encrypt(void* pDst, const void* pSrc, unsigned int size, const nn::pia::common::Crypto::Setting& setting)
{
    switch (setting.m_Mode) {
    case MODE_NONE:
        return nn::Result();
    case MODE_AES128: {
        nn::crypto::Aes<16> aes;
        nn::Result result = SetKey(aes, setting);
        if (result.IsFailure()) {
            return result;
        }
        if (size % AES_BLOCK_SIZE != 0) {
            return RESULT_INVALID_ARGUMENT;
        }
        u8* pDstBlock = static_cast<u8*>(pDst);
        const u8* pSrcBlock = static_cast<const u8*>(pSrc);
        for (u32 i = 0; i < size / AES_BLOCK_SIZE; i++) {
            aes.Encrypt(pDstBlock, pSrcBlock);
            pDstBlock += AES_BLOCK_SIZE;
            pSrcBlock += AES_BLOCK_SIZE;
        }
        return nn::Result();
    }
    default:
        return RESULT_INVALID_ARGUMENT;
    }
}

} // namespace Crypto
} // namespace common
} // namespace pia
} // namespace nn
