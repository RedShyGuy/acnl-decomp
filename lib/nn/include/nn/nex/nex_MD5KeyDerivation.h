#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyDerivation.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16MD5KeyDerivationE @ 0x008CE5C0
// vtable 0x008FD18C (vptr 0x008FD194), offset_to_top 0, 8 entries
class MD5KeyDerivation : public ::nn::nex::KeyDerivation
{
public:
    MD5KeyDerivation(); // ctor address unknown
    virtual ~MD5KeyDerivation(); // 0x00382518 slot 0x00 | slot vf_0x00 of nn::nex::MD5KeyDerivation
    virtual void vf_0x04(); // 0x00382514 slot 0x04 | virtual slot, introduced by nn::nex::MD5KeyDerivation
    virtual void CreateKey(unsigned, const char*); // 0x003824C0 slot 0x08 | mk7dlp:bytes-fuzzy
    virtual void vf_0x0C(); // 0x003823C0 slot 0x0C | virtual slot, introduced by nn::nex::MD5KeyDerivation
    virtual void vf_0x10(); // 0x0038230C slot 0x10 | virtual slot, introduced by nn::nex::MD5KeyDerivation
    virtual void CreateKey(nn::nex::CallContext*, unsigned int, const char*, nn::nex::Key*, nn::nex::KeyCache*); // 0x00382450 slot 0x14 | fefates:bytes
    virtual void InitializeKey(nn::nex::CallContext*, const char*, nn::nex::Key*); // 0x00382400 slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x00382230 slot 0x1C | virtual slot, introduced by nn::nex::MD5KeyDerivation
};
} // namespace nex
} // namespace nn
