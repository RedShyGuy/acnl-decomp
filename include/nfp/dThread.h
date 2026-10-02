#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace nfp {
// RTTI N3nfp6ThreadE @ 0x008D0FFC
// vtable 0x009046DC (vptr 0x009046E4), offset_to_top 0, 18 entries
// vtable 0x0090472C (vptr 0x00904734), offset_to_top -24, 1 entries
class Thread : public ::sead::Thread
{
public:
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x0051BA64 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0051BA38 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x0051B9E8 slot 0x40 | virtual slot, introduced by sead::Thread
};
} // namespace nfp
