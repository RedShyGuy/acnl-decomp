#pragma once

#include "decomp.h"
#include "Bs/dBsScene.h"
#include "Utl/dUtlBase.h"

// RTTI 7ScStage @ 0x008CD3E0
// vtable 0x008F919C (vptr 0x008F91A4), offset_to_top 0, 23 entries
class ScStage : public ::UtlBase<BsScene>
{
public:
    ScStage(); // ctor address unknown
    virtual ~ScStage(); // 0x0061373C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006136B0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x008209A4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820A40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006134F4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006134EC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0075EAE4 slot 0x40 | virtual slot, introduced by BsScene
    virtual void vf_0x44(); // 0x00612394 slot 0x44 | virtual slot, introduced by ScStage
    virtual void vf_0x48(); // 0x00611ECC slot 0x48 | virtual slot, introduced by ScStage
    virtual void vf_0x4C(); // 0x00611B8C slot 0x4C | virtual slot, introduced by ScStage
    virtual void vf_0x50(); // 0x00612E18 slot 0x50 | virtual slot, introduced by ScStage
    virtual void vf_0x54(); // 0x00612D48 slot 0x54 | virtual slot, introduced by ScStage
    virtual void vf_0x58(); // 0x00612B20 slot 0x58 | virtual slot, introduced by ScStage
    void CreateIslandFieldHeap(sead::Heap*); // 0x0010C404 | libgarden [tier A]
};
