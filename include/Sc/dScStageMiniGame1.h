#pragma once

#include "decomp.h"
#include "Bs/dBsScene.h"
#include "Utl/dUtlBase.h"

// RTTI 16ScStageMiniGame1 @ 0x008CC3B0
// vtable 0x008F2D84 (vptr 0x008F2D8C), offset_to_top 0, 23 entries
class ScStageMiniGame1 : public ::UtlBase<BsScene>
{
public:
    ScStageMiniGame1(); // ctor candidate(s) 0x007F0430 (unverified)
    virtual ~ScStageMiniGame1(); // 0x002BCEB0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002BCEA0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002BCE5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002BCE54 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0071DF84 slot 0x40 | virtual slot, introduced by BsScene
    bool StartInitialize() override; // 0x002BCCC8 slot 0x44
    oml::framework::Result UpdateInitialize() override; // 0x002BCC80 slot 0x48
    bool EndInitialize() override; // 0x002BCBEC slot 0x4C
    bool StartFinalize() override; // 0x002BCE3C slot 0x50
    oml::framework::Result UpdateFinalize() override; // 0x002BCE24 slot 0x54
    bool EndFinalize() override; // 0x002BCDB4 slot 0x58
};
