#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common6StringE @ 0x008CFED0
// vtable 0x00901608 (vptr 0x00901610), offset_to_top 0, 1 entries
class String : public ::nn::pia::common::RootObject
{
public:
    String(); // ctor candidate(s) 0x003E87E8, 0x004293C0 (unverified)
    virtual void vf_0x00(); // 0x007333D4 slot 0x00 | virtual slot, introduced by nn::pia::common::String
    void StrLen() const; // 0x002FA990 | fefates:bytes [tier B]
    void Format(const char*, ...); // 0x00429388 | fefates:bytes [tier B]
    String(const char*); // 0x004293C0 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
