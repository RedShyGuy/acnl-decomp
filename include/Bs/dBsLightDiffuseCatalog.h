#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixBase.h"

// RTTI 21BsLightDiffuseCatalog @ 0x008CCC68
// vtable 0x008F5F8C (vptr 0x008F5F94), offset_to_top 0, 21 entries
class BsLightDiffuseCatalog : public ::BsLightFixBase
{
public:
    BsLightDiffuseCatalog(); // ctor candidate(s) 0x007F2278 (unverified)
    virtual ~BsLightDiffuseCatalog(); // 0x00325A10 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x003259EC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0032594C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x003259DC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x003259A4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00325924 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x003259CC slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x50(); // 0x00325920 slot 0x50 | virtual slot, introduced by BsLightFixBase
};
