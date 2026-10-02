#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"

// RTTI 17ResourceGetSklMat @ 0x008CC6AC
// vtable 0x008437E8 (vptr 0x008437F0), offset_to_top 0, 9 entries
// vtable 0x008F3C00 (vptr 0x008F3C08), offset_to_top 0, 9 entries
// vtable 0x008F3C54 (vptr 0x008F3C5C), offset_to_top -80, 11 entries
// vtable 0x008438C8 (vptr 0x008438D0), offset_to_top -780, 11 entries
class ResourceGetSklMat : public ::ResourceGetSkeletal
{
public:
    ResourceGetSklMat(); // ctor address unknown
    virtual void vf_0x00(); // 0x002D1380 slot 0x00 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x04(); // 0x002D1328 slot 0x04 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x08(); // 0x002D0F90 slot 0x08 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x0C(); // 0x002D1174 slot 0x0C | virtual slot, introduced by ResourceGetSkeletal
};
