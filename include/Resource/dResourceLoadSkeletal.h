#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"

// RTTI 20ResourceLoadSkeletal @ 0x008CCC00
// vtable 0x00882684 (vptr 0x0088268C), offset_to_top 0, 11 entries
// vtable 0x008F5A8C (vptr 0x008F5A94), offset_to_top 0, 11 entries
// vtable 0x0088276C (vptr 0x00882774), offset_to_top -312, 11 entries
// vtable 0x008F5AE8 (vptr 0x008F5AF0), offset_to_top -312, 11 entries
class ResourceLoadSkeletal : public ::ResourceGetSkeletal
{
public:
    ResourceLoadSkeletal(); // ctor address unknown
    virtual void vf_0x00(); // 0x00321F20 slot 0x00 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x04(); // 0x00321F0C slot 0x04 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x0C(); // 0x00321E2C slot 0x0C | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x10(); // 0x00321D8C slot 0x10 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x24(); // 0x00321DC0 slot 0x24 | virtual slot, introduced by ResourceLoadSkeletal
    virtual void vf_0x28(); // 0x00321D88 slot 0x28 | virtual slot, introduced by ResourceLoadSkeletal
};
