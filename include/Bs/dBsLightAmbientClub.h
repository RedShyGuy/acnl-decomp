#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 18BsLightAmbientClub @ 0x008CC734
// vtable 0x008F4010 (vptr 0x008F4018), offset_to_top 0, 20 entries
class BsLightAmbientClub : public ::BsLightBase
{
public:
    class AmbientClubLightHioNode;
    BsLightAmbientClub(); // ctor address unknown
    virtual ~BsLightAmbientClub(); // 0x002D7174 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002D7150 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002D6D54 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002D7104 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002D7064 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002D6D00 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x007210B4 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x002D7134 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x002D70F4 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x002D6CFC slot 0x4C | virtual slot, introduced by BsLightAmbientClub
};
