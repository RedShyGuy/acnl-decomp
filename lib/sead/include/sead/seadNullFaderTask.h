#pragma once

#include "decomp.h"
#include "sead/seadFaderTaskBase.h"

namespace sead {
// RTTI N4sead13NullFaderTaskE @ 0x008D167C
// vtable 0x00905608 (vptr 0x00905610), offset_to_top 0, 26 entries
class NullFaderTask : public ::sead::FaderTaskBase
{
public:
    NullFaderTask(); // ctor candidate(s) 0x0054323C (unverified)
    virtual ~NullFaderTask(); // 0x00543288 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00543278 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void pauseDraw(bool); // 0x00543238 slot 0x14 | slot vf_0x14 of sead::TaskBase
    virtual void vf_0x1C(); // 0x0054321C slot 0x1C | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x24(); // 0x00543228 slot 0x24 | virtual slot, introduced by sead::TaskBase
    virtual void attachDrawImpl(); // 0x00543220 slot 0x40 | slot vf_0x40 of sead::TaskBase
    virtual void detachDrawImpl(); // 0x00543224 slot 0x48 | slot vf_0x48 of sead::TaskBase
    virtual void vf_0x4C(); // 0x0074B9F8 slot 0x4C | virtual slot, introduced by sead::TaskBase
    virtual void getMethodTreeNode(int); // 0x0054322C slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void vf_0x64(); // 0x00543234 slot 0x64 | virtual slot, introduced by sead::NullFaderTask
};
} // namespace sead
