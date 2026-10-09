#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_ShaBlock512BitContext.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto13Sha256ContextE @ 0x008D04B4
// vtable 0x009022CC (vptr 0x009022D4), offset_to_top 0, 9 entries
//
// SHA-256 (FIPS 180). The slot names follow Sha1Context; the member name is ours.
class Sha256Context : public ::nn::crypto::ShaBlock512BitContext
{
public:
    static const size_t HASH_SIZE = 32;

    Sha256Context() {}

    virtual void Initialize(); // 0x0048331C slot 0x00
    virtual void Finalize(); // 0x00483490 slot 0x04 (name is ours)
    virtual void Update(const void* pData, size_t size); // 0x00483418 slot 0x08
    virtual size_t GetHashSize() const; // 0x00483388 slot 0x0C (name is ours)
    virtual void GetHash(void* pOutput); // 0x0048341C slot 0x10
    virtual ~Sha256Context(); // slots 0x14, 0x18
    virtual void ProcessBlock(); // 0x00148D70 slot 0x1C
    virtual void InitializeWithState(const void* pState, u64 size); // 0x00483390 slot 0x20 (name is ours)

    u32 m_State[8]; // 0x50
};
ASSERT_SIZE(Sha256Context, 0x70);
} // namespace crypto
} // namespace nn
