#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

namespace mutil {
// RTTI N5mutil13LoaderWrapperE @ 0x008D2CF8
// vtable 0x00909174 (vptr 0x0090917C), offset_to_top 0, 4 entries
class LoaderWrapper : public ::ssys::ma::LoadSplit
{
public:
    LoaderWrapper(); // ctor address unknown
    virtual ~LoaderWrapper(); // 0x005B2470 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005B2460 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
} // namespace mutil
