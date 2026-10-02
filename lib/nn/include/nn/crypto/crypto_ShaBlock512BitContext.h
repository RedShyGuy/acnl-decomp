#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto21ShaBlock512BitContextE @ 0x008D04C8
class ShaBlock512BitContext : public ::nn::crypto::HashContextBase
{
public:
    ShaBlock512BitContext(); // ctor address unknown
    void AddPadding(); // 0x0014375C | nintendogs:bytes [tier A]
    void Update(const void*, unsigned); // 0x001437E4 | nintendogs:bytes [tier A]
};
} // namespace crypto
} // namespace nn
