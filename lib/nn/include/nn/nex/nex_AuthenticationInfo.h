#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_AuthenticationInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18AuthenticationInfoE @ 0x008CE6E8
// vtable 0x008FD468 (vptr 0x008FD470), offset_to_top 0, 8 entries
class AuthenticationInfo : public ::nn::nex::_DDL_AuthenticationInfo
{
public:
    AuthenticationInfo(); // ctor candidate(s) 0x00357A5C, 0x0035D194, 0x003D725C (unverified)
    virtual void vf_0x00(); // 0x00387420 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~AuthenticationInfo(); // 0x003873FC slot 0x04 | slot vf_0x04 of nn::nex::Data
};
} // namespace nex
} // namespace nn
