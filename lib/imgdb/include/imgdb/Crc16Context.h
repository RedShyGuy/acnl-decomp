#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace imgdb {
// RTTI N5imgdb12Crc16ContextE @ 0x008D2AE4
// vtable 0x00908EE0 (vptr 0x00908EE8), offset_to_top 0, 9 entries
// (slots 0x0C and 0x1C are the defaults of nn::crypto::HashContextBase; slot names after it)
class Crc16Context : public ::nn::crypto::HashContextBase
{
public:
    Crc16Context(); // ctor candidate(s) 0x005B03B0 (unverified)
    virtual void Initialize(); // 0x005A5718 slot 0x00
    virtual void Finalize(); // 0x005A57C8 slot 0x04
    virtual void Update(const void* pData, size_t size); // 0x005A5734 slot 0x08
    virtual void GetHash(void* pOutput); // 0x005A57BC slot 0x10
    virtual ~Crc16Context(); // slots 0x14, 0x18
    virtual size_t vf_0x20(); // 0x005A572C slot 0x20 | virtual slot, introduced by imgdb::Crc16Context
};
} // namespace imgdb
