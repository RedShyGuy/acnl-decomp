#pragma once

#include "decomp.h"
#include "Resource/dResourceLoadSkeletal.h"

// RTTI 25ResourceLoadAsyncSkeletal @ 0x008CD06C
// vtable 0x008F7CD4 (vptr 0x008F7CDC), offset_to_top 0, 11 entries
// vtable 0x008F7D30 (vptr 0x008F7D38), offset_to_top -312, 11 entries
class ResourceLoadAsyncSkeletal : public ::ResourceLoadSkeletal
{
public:
    ResourceLoadAsyncSkeletal(); // ctor address unknown
    virtual void vf_0x00(); // 0x0033FB7C slot 0x00 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x04(); // 0x0033FB38 slot 0x04 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x24(); // 0x0033FA1C slot 0x24 | virtual slot, introduced by ResourceLoadSkeletal
};
