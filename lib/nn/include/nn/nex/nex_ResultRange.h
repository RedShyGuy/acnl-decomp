#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_ResultRange.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11ResultRangeE @ 0x008CE080
// vtable 0x008FC43C (vptr 0x008FC444), offset_to_top 0, 4 entries
class ResultRange : public ::nn::nex::_DDL_ResultRange
{
public:
    ResultRange(); // ctor candidate(s) 0x0050E8D0 (unverified)
    // (inline in pia::inet::NexMatchmakeSession)
    ResultRange(u32 offset, u32 size)
    {
        m_Offset = offset;
        m_Size = size;
    }
    virtual void vf_0x00(); // 0x0035BA7C slot 0x00 | virtual slot, introduced by nn::nex::_DDL_ResultRange
    virtual void vf_0x04(); // 0x0035BA78 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_ResultRange
    virtual void vf_0x08(); // 0x0072A388 slot 0x08 | virtual slot, introduced by nn::nex::ResultRange
    virtual void vf_0x0C(); // 0x0072A380 slot 0x0C | virtual slot, introduced by nn::nex::ResultRange
};
} // namespace nex
} // namespace nn
