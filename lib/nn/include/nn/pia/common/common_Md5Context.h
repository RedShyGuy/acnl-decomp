#pragma once

#include "decomp.h"
#include "nn/pia/common/common_HashContextBase.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common10Md5ContextE @ 0x008CFDFC
// vtable 0x0090150C (vptr 0x00901514), offset_to_top 0, 6 entries
class Md5Context : public ::nn::pia::common::HashContextBase
{
public:
    Md5Context(); // ctor candidate(s) 0x00426C68, 0x00427B64 (unverified)
    virtual void vf_0x00(); // 0x004261BC slot 0x00 | fefates:callgraph
    virtual void Update(const void*, unsigned int); // 0x00426504 slot 0x04 | fefates:bytes
    virtual void GetHashSize() const; // 0x0073180C slot 0x08 | slot vf_0x08 of nn::pia::common::Md5Context
    virtual void vf_0x0C(); // 0x00731814 slot 0x0C | virtual slot, introduced by nn::pia::common::Md5Context
    virtual void GetHash(void*); // 0x004265D0 slot 0x10 | fefates:bytes
    virtual void ProcessBlock(); // 0x004261EC slot 0x14 | fefates:bytes
};
} // namespace common
} // namespace pia
} // namespace nn
