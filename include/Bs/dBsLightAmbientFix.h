#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 17BsLightAmbientFix @ 0x008CC474
// vtable 0x008F3498 (vptr 0x008F34A0), offset_to_top 0, 20 entries
class BsLightAmbientFix : public ::BsLightBase
{
public:
    class AmbientFixLightHioNode;
    BsLightAmbientFix(); // ctor address unknown
    virtual ~BsLightAmbientFix(); // 0x002C768C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C7668 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C729C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002C762C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002C75F0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C7248 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0071F908 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x002C764C slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x002C761C slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x002C7220 slot 0x4C | virtual slot, introduced by BsLightAmbientFix
};
