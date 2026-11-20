#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsSnowBallMgr @ 0x008CB970
// vtable 0x008EF08C (vptr 0x008EF094), offset_to_top 0, 22 entries
class BsSnowBallMgr : public ::UtlBase<Base>
{
public:
    BsSnowBallMgr(); // ctor candidate(s) 0x0022705C (unverified)
    virtual ~BsSnowBallMgr(); // 0x002270B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0022708C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x00226F5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x00226F54 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x00225CF8 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00225C9C slot 0x44
    bool EndInitialize() override; // 0x00225C64 slot 0x48
    bool StartFinalize() override; // 0x0022616C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x00226164 slot 0x50
    bool EndFinalize() override; // 0x00225EE4 slot 0x54

    // (game code, not decompiled yet; names are ours, after BsNpcMgr) something is built / previewed on the units of the
    // rectangle x0..x1, z0..z1: the snowballs there react
    void OnBuild(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00225DE8
    void OnPreview(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00226434
    void OnPreviewEnd(); // 0x00226D0C

    static BsSnowBallMgr* s_pInstance;
};
