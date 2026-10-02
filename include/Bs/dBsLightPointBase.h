#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 16BsLightPointBase @ 0x008CC1D4
// vtable 0x008F23F8 (vptr 0x008F2400), offset_to_top 0, 20 entries
class BsLightPointBase : public ::BsLightBase
{
public:
    class PointLightHioNode;
    BsLightPointBase(); // ctor address unknown
    virtual ~BsLightPointBase(); // 0x002AE48C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002AE460 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002AE0DC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002AE414 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002AE1D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002AE078 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0071D8E4 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x002AE444 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x002ADF84 slot 0x4C | virtual slot, introduced by BsLightPointBase
};
