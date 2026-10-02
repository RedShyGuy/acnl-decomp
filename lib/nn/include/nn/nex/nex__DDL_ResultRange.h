#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16_DDL_ResultRangeE @ 0x008CE644
// vtable 0x008FD2C8 (vptr 0x008FD2D0), offset_to_top 0, 2 entries
class _DDL_ResultRange : public ::nn::nex::RootObject
{
public:
    _DDL_ResultRange(); // ctor address unknown
    virtual void vf_0x00(); // 0x00384100 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_ResultRange
    virtual void vf_0x04(); // 0x003840FC slot 0x04 | virtual slot, introduced by nn::nex::_DDL_ResultRange
    void Add(nn::nex::Message*, const nn::nex::_DDL_ResultRange&); // 0x00384024 | fefates:bytes [tier B]
    void operator=(const nn::nex::_DDL_ResultRange&); // 0x00384104 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
