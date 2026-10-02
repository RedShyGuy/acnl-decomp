#pragma once

#include "decomp.h"
#include "Bs/dBsScene.h"
#include "Utl/dUtlBase.h"

// RTTI 16ScStageMiniGame0 @ 0x008CC3A4
// vtable 0x008F2D20 (vptr 0x008F2D28), offset_to_top 0, 23 entries
class ScStageMiniGame0 : public ::UtlBase<BsScene>
{
public:
    ScStageMiniGame0(); // ctor candidate(s) 0x007F03F4 (unverified)
    virtual ~ScStageMiniGame0(); // 0x00313AF0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002BCBDC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x008209A4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820A40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002BCB98 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002BCB90 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0071DF7C slot 0x40 | virtual slot, introduced by BsScene
    virtual void vf_0x44(); // 0x002BC990 slot 0x44 | virtual slot, introduced by ScStageMiniGame0
    virtual void vf_0x48(); // 0x002BC948 slot 0x48 | virtual slot, introduced by ScStageMiniGame0
    virtual void vf_0x4C(); // 0x002BC8A4 slot 0x4C | virtual slot, introduced by ScStageMiniGame0
    virtual void vf_0x50(); // 0x002BCB68 slot 0x50 | virtual slot, introduced by ScStageMiniGame0
    virtual void vf_0x54(); // 0x002BCB50 slot 0x54 | virtual slot, introduced by ScStageMiniGame0
    virtual void vf_0x58(); // 0x002BCAB4 slot 0x58 | virtual slot, introduced by ScStageMiniGame0
};
