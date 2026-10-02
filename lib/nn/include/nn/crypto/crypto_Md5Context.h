#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto10Md5ContextE @ 0x008D047C
// vtable 0x0090220C (vptr 0x00902214), offset_to_top 0, 9 entries
class Md5Context : public ::nn::crypto::HashContextBase
{
public:
    Md5Context(); // ctor candidate(s) 0x0035CC58, 0x0035D6D4, 0x0035EF80, 0x00374614, 0x003D5830, 0x0072AFA8 (unverified)
    virtual void vf_0x00(); // 0x00482820 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x00482DEC slot 0x04 | virtual slot, introduced by nn::crypto::Md5Context
    virtual void Update(const void*, unsigned int); // 0x00482B9C slot 0x08 | fefates:bytes
    virtual void vf_0x0C(); // 0x00482850 slot 0x0C | virtual slot, introduced by nn::crypto::Md5Context
    virtual void GetHash(void*); // 0x00482C68 slot 0x10 | fefates:bytes
    virtual void vf_0x14(); // 0x00482DF4 slot 0x14 | virtual slot, introduced by nn::crypto::Md5Context
    virtual void vf_0x18(); // 0x00482DF0 slot 0x18 | virtual slot, introduced by nn::crypto::Md5Context
    virtual void ProcessBlock(); // 0x00482858 slot 0x1C | fefates:bytes
    virtual void vf_0x20(); // 0x00482B90 slot 0x20 | virtual slot, introduced by nn::crypto::Md5Context
};
} // namespace crypto
} // namespace nn
