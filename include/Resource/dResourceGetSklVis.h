#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"

// RTTI 17ResourceGetSklVis @ 0x008CC6B8
// vtable 0x008824E0 (vptr 0x008824E8), offset_to_top 0, 9 entries
// vtable 0x008F3C9C (vptr 0x008F3CA4), offset_to_top 0, 9 entries
// vtable 0x008F3CF0 (vptr 0x008F3CF8), offset_to_top -72, 11 entries
// vtable 0x008825C0 (vptr 0x008825C8), offset_to_top -104, 11 entries
class ResourceGetSklVis : public ::ResourceGetSkeletal
{
public:
    ResourceGetSklVis(); // ctor address unknown
    virtual void vf_0x00(); // 0x002D1864 slot 0x00 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x04(); // 0x002D180C slot 0x04 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x08(); // 0x002D13E4 slot 0x08 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x0C(); // 0x002D15D0 slot 0x0C | virtual slot, introduced by ResourceGetSkeletal
};
