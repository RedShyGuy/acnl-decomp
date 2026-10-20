#pragma once

#include "decomp.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex__DDL_ResultRange.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11ResultRangeE @ 0x008CE080
// vtable 0x008FC43C (vptr 0x008FC444), offset_to_top 0, 4 entries
class ResultRange : public ::nn::nex::_DDL_ResultRange
{
public:
    // the size of a default range (the name is ours)
    static const u32 DEFAULT_SIZE = 20;

    // (inline; the first 20)
    ResultRange()
    {
        m_uiOffset = 0;
        m_uiSize = DEFAULT_SIZE;
    }
    // (inline in pia::inet::NexMatchmakeSession)
    ResultRange(u32 offset, u32 size)
    {
        m_uiOffset = offset;
        m_uiSize = size;
    }
    virtual ~ResultRange(); // 0x0035BA7C slot 0x00
    // 0x0035BA78 slot 0x04 (deleting dtor)
    // (inline, with copies out of line; the names are ours)
    virtual u32 GetOffset() const { return m_uiOffset; } // 0x0072A388 slot 0x08
    virtual u32 GetSize() const { return m_uiSize; } // 0x0072A380 slot 0x0C
};
ASSERT_SIZE(ResultRange, 0x10);

} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void list<nn::nex::ResultRange, nn::nex::MemAllocator<nn::nex::ResultRange> >::clear();
} // namespace std
