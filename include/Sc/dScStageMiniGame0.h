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
    virtual oml::framework::Result Calc(); // 0x002BCB98 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002BCB90 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0071DF7C slot 0x40 | virtual slot, introduced by BsScene
    bool StartInitialize() override; // 0x002BC990 slot 0x44
    oml::framework::Result UpdateInitialize() override; // 0x002BC948 slot 0x48
    bool EndInitialize() override; // 0x002BC8A4 slot 0x4C
    bool StartFinalize() override; // 0x002BCB68 slot 0x50
    oml::framework::Result UpdateFinalize() override; // 0x002BCB50 slot 0x54
    bool EndFinalize() override; // 0x002BCAB4 slot 0x58
};
