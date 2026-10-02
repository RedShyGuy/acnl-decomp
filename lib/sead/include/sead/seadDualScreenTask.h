#pragma once

#include "decomp.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead14DualScreenTaskE @ 0x008D16F8
// vtable 0x009057C4 (vptr 0x009057CC), offset_to_top 0, 25 entries
class DualScreenTask : public ::sead::TaskBase
{
public:
    DualScreenTask(); // ctor address unknown
    virtual ~DualScreenTask(); // 0x00544F68 slot 0x00 | nintendogs:bytes
    // 0x00544EFC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074BF14 slot 0x08 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x0C(); // 0x0074BEC8 slot 0x0C | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x10(); // 0x00544DB0 slot 0x10 | virtual slot, introduced by sead::TaskBase
    virtual void pauseDraw(bool); // 0x00544E18 slot 0x14 | nintendogs:bytes
    virtual void vf_0x18(); // 0x00544928 slot 0x18 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x1C(); // 0x00544990 slot 0x1C | virtual slot, introduced by sead::TaskBase
    virtual void attachCalcImpl(); // 0x00544A74 slot 0x3C | slot vf_0x3C of sead::TaskBase
    virtual void attachDrawImpl(); // 0x00544B00 slot 0x40 | nintendogs:bytes-fuzzy
    virtual void detachCalcImpl(); // 0x00544BD0 slot 0x44 | slot vf_0x44 of sead::TaskBase
    virtual void detachDrawImpl(); // 0x00544BD8 slot 0x48 | nintendogs:bytes
    virtual void vf_0x4C(); // 0x0074BFC8 slot 0x4C | virtual slot, introduced by sead::TaskBase
    virtual void getMethodTreeNode(int); // 0x00544C34 slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void vf_0x58(); // 0x00544DA4 slot 0x58 | virtual slot, introduced by sead::DualScreenTask
    virtual void vf_0x5C(); // 0x00544DAC slot 0x5C | virtual slot, introduced by sead::DualScreenTask
    virtual void vf_0x60(); // 0x00544DA8 slot 0x60 | virtual slot, introduced by sead::DualScreenTask
};
} // namespace sead
