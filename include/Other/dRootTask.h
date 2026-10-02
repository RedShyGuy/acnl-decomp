#pragma once

#include "decomp.h"
#include "sead/seadUlcdTask.h"

// RTTI 8RootTask @ 0x008CD56C
// vtable 0x008F9DBC (vptr 0x008F9DC4), offset_to_top 0, 27 entries
class RootTask : public ::sead::UlcdTask
{
public:
    RootTask(); // ctor address unknown
    virtual ~RootTask(); // 0x006B26C0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006B26B0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void prepare(); // 0x006B2660 slot 0x28 | slot vf_0x28 of sead::TaskBase
    virtual void enter(); // 0x006B2604 slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void vf_0x58(); // 0x006B2600 slot 0x58 | virtual slot, introduced by sead::DualScreenTask
};
