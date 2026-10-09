#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_ShaBlock512BitContext.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto11Sha1ContextE @ 0x008D0490
// vtable 0x00902238 (vptr 0x00902240), offset_to_top 0, 9 entries
//
// SHA-1 (FIPS 180). The member name is ours.
class Sha1Context : public ::nn::crypto::ShaBlock512BitContext
{
public:
    static const size_t HASH_SIZE = 20;

    Sha1Context() {}

    virtual void Initialize(); // 0x00482DF8 slot 0x00 | nintendogs:bytes
    virtual void Finalize(); // 0x0048320C slot 0x04 (name is ours)
    virtual void Update(const void* pData, size_t size); // 0x001437E0 slot 0x08
    virtual size_t GetHashSize() const; // 0x00482E44 slot 0x0C (name is ours)
    virtual void GetHash(void* pOutput); // 0x004831BC slot 0x10 | nintendogs:bytes
    virtual ~Sha1Context(); // slots 0x14, 0x18
    virtual void ProcessBlock(); // 0x00482E4C slot 0x1C | nintendogs:bytes
    virtual void InitializeWithState(const void* pState, u64 size); // 0x00483158 slot 0x20 (name is ours)

    u32 m_State[5]; // 0x50
};
ASSERT_SIZE(Sha1Context, 0x64);
} // namespace crypto
} // namespace nn
