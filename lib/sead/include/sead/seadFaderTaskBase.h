#pragma once

#include "decomp.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead13FaderTaskBaseE @ 0x008D1650
// vtable 0x00905500 (vptr 0x00905508), offset_to_top 0, 25 entries
class FaderTaskBase : public ::sead::TaskBase
{
public:
    FaderTaskBase(); // ctor address unknown
    virtual ~FaderTaskBase(); // 0x00542C84 slot 0x00 | nintendogs:callseq
    // 0x00542C74 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x10(); // 0x005429B4 slot 0x10 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x18(); // 0x00542254 slot 0x18 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x20(); // 0x00542358 slot 0x20 | virtual slot, introduced by sead::TaskBase
    virtual void enter(); // 0x005425D8 slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void attachCalcImpl(); // 0x005422E8 slot 0x3C | nintendogs:callseq
    virtual void detachCalcImpl(); // 0x00542350 slot 0x44 | slot vf_0x44 of sead::TaskBase
    virtual void getMethodTreeNode(int); // 0x005424A4 slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void calcCore_(); // 0x0054260C slot 0x58 | nintendogs:callseq
    virtual void vf_0x5C(); // 0x00542250 slot 0x5C | virtual slot, introduced by sead::FaderTaskBase
    virtual void vf_0x60(); // 0x005424B8 slot 0x60 | virtual slot, introduced by sead::FaderTaskBase
};
} // namespace sead
