#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// A clock that starts when it is made. The member name is ours.
// RTTI N2nn3nex10LocalClockE @ 0x008CDFB0
// vtable 0x008FC1B0 (vptr 0x008FC1B8), offset_to_top 0, 2 entries
class LocalClock : public ::nn::nex::RootObject
{
public:
    // (inline)
    LocalClock() { m_StartTime = SystemClock::GetTime(); }
    virtual ~LocalClock(); // 0x003550F8 slot 0x00
    // 0x003550D4 slot 0x04 (deleting dtor)

    Time m_StartTime; // 0x08
};
ASSERT_SIZE(LocalClock, 0x10);
} // namespace nex
} // namespace nn
