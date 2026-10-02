#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "script/dITalkRecept.h"

// RTTI 17AcInsectFishPlate @ 0x008CC424
// vtable 0x008F3044 (vptr 0x008F304C), offset_to_top 0, 30 entries
// vtable 0x008F30C4 (vptr 0x008F30CC), offset_to_top -104, 63 entries
class AcInsectFishPlate : public ::DemoActor, public ::script::ITalkRecept
{
public:
    AcInsectFishPlate(); // ctor candidate(s) 0x007F056C (unverified)
    virtual ~AcInsectFishPlate(); // 0x002C5050 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C5034 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C4C4C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002C4DCC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002C4CB8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C4C44 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x0071F8F8 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x50(); // 0x002C4DE8 slot 0x50 | virtual slot, introduced by DemoActor
};
