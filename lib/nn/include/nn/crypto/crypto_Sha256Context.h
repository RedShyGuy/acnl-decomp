#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_ShaBlock512BitContext.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto13Sha256ContextE @ 0x008D04B4
// vtable 0x009022CC (vptr 0x009022D4), offset_to_top 0, 9 entries
class Sha256Context : public ::nn::crypto::ShaBlock512BitContext
{
public:
    Sha256Context(); // ctor candidate(s) 0x00140D00 (unverified)
    virtual void vf_0x00(); // 0x0048331C slot 0x00 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x04(); // 0x00483490 slot 0x04 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x08(); // 0x00483418 slot 0x08 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x0C(); // 0x00483388 slot 0x0C | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x10(); // 0x0048341C slot 0x10 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x14(); // 0x00483498 slot 0x14 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x18(); // 0x00483494 slot 0x18 | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x1C(); // 0x00148D70 slot 0x1C | virtual slot, introduced by nn::crypto::Sha256Context
    virtual void vf_0x20(); // 0x00483390 slot 0x20 | virtual slot, introduced by nn::crypto::Sha256Context
};
} // namespace crypto
} // namespace nn
