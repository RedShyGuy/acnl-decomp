#pragma once

#include "decomp.h"
#include "sead/seadUlcdTask.h"

// RTTI 10SystemTask @ 0x008CB1FC
// vtable 0x008EC510 (vptr 0x008EC518), offset_to_top 0, 27 entries
class SystemTask : public ::sead::UlcdTask
{
public:
    SystemTask(); // ctor candidate(s) 0x001B74C4 (unverified)
    virtual ~SystemTask(); // 0x00561964 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001B74E8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void prepare(); // 0x001B71E4 slot 0x28 | slot vf_0x28 of sead::TaskBase
    virtual void enter(); // 0x001B6E68 slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void exit(); // 0x001B6E64 slot 0x34 | slot vf_0x34 of sead::TaskBase
    virtual void vf_0x58(); // 0x001B6C28 slot 0x58 | virtual slot, introduced by sead::DualScreenTask
    virtual void vf_0x60(); // 0x001B7134 slot 0x60 | virtual slot, introduced by sead::DualScreenTask
    virtual void vf_0x64(); // 0x001B7268 slot 0x64 | virtual slot, introduced by sead::UlcdTask
    virtual void vf_0x68(); // 0x001B7388 slot 0x68 | virtual slot, introduced by sead::UlcdTask
};
