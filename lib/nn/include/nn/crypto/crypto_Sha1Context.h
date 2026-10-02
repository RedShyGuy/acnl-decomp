#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_ShaBlock512BitContext.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto11Sha1ContextE @ 0x008D0490
// vtable 0x00902238 (vptr 0x00902240), offset_to_top 0, 9 entries
class Sha1Context : public ::nn::crypto::ShaBlock512BitContext
{
public:
    Sha1Context(); // ctor candidate(s) 0x00483278 (unverified)
    virtual void Initialize(); // 0x00482DF8 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0048320C slot 0x04 | virtual slot, introduced by nn::crypto::Sha1Context
    virtual void vf_0x08(); // 0x001437E0 slot 0x08 | virtual slot, introduced by nn::crypto::Sha1Context
    virtual void vf_0x0C(); // 0x00482E44 slot 0x0C | virtual slot, introduced by nn::crypto::Sha1Context
    virtual void GetHash(void*); // 0x004831BC slot 0x10 | nintendogs:bytes
    virtual void vf_0x14(); // 0x00483214 slot 0x14 | virtual slot, introduced by nn::crypto::Sha1Context
    virtual void vf_0x18(); // 0x00483210 slot 0x18 | virtual slot, introduced by nn::crypto::Sha1Context
    virtual void ProcessBlock(); // 0x00482E4C slot 0x1C | nintendogs:bytes
    virtual void vf_0x20(); // 0x00483158 slot 0x20 | virtual slot, introduced by nn::crypto::Sha1Context
};
} // namespace crypto
} // namespace nn
