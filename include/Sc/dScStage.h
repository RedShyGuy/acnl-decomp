#pragma once

#include "decomp.h"
#include "Bs/dBsScene.h"
#include "Utl/dUtlBase.h"
#include "nn/math/math_Vector3.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead

// RTTI 7ScStage @ 0x008CD3E0
// vtable 0x008F919C (vptr 0x008F91A4), offset_to_top 0, 23 entries
class ScStage : public ::UtlBase<BsScene>
{
public:
    ScStage(); // ctor address unknown
    virtual ~ScStage(); // 0x0061373C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006136B0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x006134F4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006134EC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0075EAE4 slot 0x40 | virtual slot, introduced by BsScene
    bool StartInitialize() override; // 0x00612394 slot 0x44
    oml::framework::Result UpdateInitialize() override; // 0x00611ECC slot 0x48
    bool EndInitialize() override; // 0x00611B8C slot 0x4C
    bool StartFinalize() override; // 0x00612E18 slot 0x50
    oml::framework::Result UpdateFinalize() override; // 0x00612D48 slot 0x54
    bool EndFinalize() override; // 0x00612B20 slot 0x58
    // the heap "IslandFieldHeap" in parent (HeapInit::s_pIslandFieldHeap; Thumb code in dHeapInitThumb.cpp)
    static sead::ExpHeap* CreateIslandFieldHeap(sead::Heap* parent); // 0x0010C40C | libgarden [tier A]

    // (names are ours; only what the stage code reads)
    u32 _unknown[(0x420 - sizeof(::UtlBase<BsScene>)) / 4];
    u8 m_Work[0x968];               // 0x420 a part that the stage code hands out (type unknown)
    bool m_HasArea;                 // 0xD88 the field area below is set
    nn::math::VEC3_ m_AreaMin;      // 0xD8C
    nn::math::VEC3_ m_AreaMax;      // 0xD98
    nn::math::VEC3_ m_AreaCenter;   // 0xDA4
};
ASSERT_OFFSET(ScStage, m_Work, 0x420);

// the frames of the stage scene (ScStage counts them; the field objects start animations with it; name is ours)
extern s32 g_StageFrameCounter;
ASSERT_OFFSET(ScStage, m_HasArea, 0xD88);
