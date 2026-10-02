#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace qrdecode {
// RTTI N8qrdecode6ThreadE @ 0x008D4084
// vtable 0x0090C198 (vptr 0x0090C1A0), offset_to_top 0, 18 entries
// vtable 0x0090C1E8 (vptr 0x0090C1F0), offset_to_top -24, 1 entries
class Thread : public ::sead::Thread
{
public:
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x006CAD0C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006CACFC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x006CACC4 slot 0x40 | virtual slot, introduced by sead::Thread
};
} // namespace qrdecode
