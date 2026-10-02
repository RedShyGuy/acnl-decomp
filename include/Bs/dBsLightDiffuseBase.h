#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 18BsLightDiffuseBase @ 0x008CC740
// vtable 0x008F4068 (vptr 0x008F4070), offset_to_top 0, 26 entries
class BsLightDiffuseBase : public ::BsLightBase
{
public:
    class DiffuseLightHioNode;
    BsLightDiffuseBase(); // ctor address unknown
    virtual ~BsLightDiffuseBase(); // 0x002D7744 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002D7720 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002D7460 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002D758C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002D750C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002D7414 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x007210F4 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x002D7678 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x007210D4 slot 0x4C | virtual slot, introduced by BsLightDiffuseBase
    virtual void vf_0x50(); // 0x002D73F8 slot 0x50 | virtual slot, introduced by BsLightDiffuseBase
    virtual void vf_0x54(); // 0x002D728C slot 0x54 | virtual slot, introduced by BsLightDiffuseBase
    virtual void vf_0x58(); // 0x002D75AC slot 0x58 | virtual slot, introduced by BsLightDiffuseBase
    virtual void vf_0x5C(); // 0x0011C12F slot 0x5C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x60(); // 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x64(); // 0x002D7194 slot 0x64 | virtual slot, introduced by BsLightDiffuseBase
};
