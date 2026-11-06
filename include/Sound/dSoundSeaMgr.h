#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/snd/snd_SoundHandle.h"

namespace sead { class Heap; }

// The sound of the sea: volume and panning from the position of the listener, a separate calculation per kind of stage
// (the class name is from the RTTI of its disposer, everything else is ours).
class SoundSeaMgr
{
public:
    class SingletonDisposer_;

    // m_Type: which calculation is used
    enum Type
    {
        TYPE_VILLAGE = 0, // the village, or a tour with TourResult state 0
        TYPE_DOWNTOWN = 1,
        TYPE_TOUR_1 = 2, // a tour with TourResult state 1
        TYPE_TOUR_2 = 3, // a tour with TourResult state 2
        TYPE_ISLAND = 4,
        TYPE_NUM = 5,
    };

    SoundSeaMgr();
    static SoundSeaMgr* createInstance(sead::Heap* heap); // 0x001DA7C4
    static void destroyInstance(); // 0x00123500

    void SetType(u8 type); // 0x001DA7BC
    void SetOceanSide(bool side); // 0x001DA858
    void CalcTour2(); // 0x001DA92C
    void CalcIsland(); // 0x001DAC54
    void CalcVillage(); // 0x001DAD38
    void CalcDownTown(); // 0x001DB0F8
    void CalcTour1(); // 0x001DB314
    void Update(); // 0x001DB418
    void OnEnterStage(); // 0x001DB480

    static SoundSeaMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    // nothing reads it
    static nn::math::VEC2 s_Unused;
    static void (SoundSeaMgr::*const s_CalcFuncs[TYPE_NUM])();

    u32 m_SingletonDisposerBuffer[4];     // 0x00
    u8 m_Type;                            // 0x10 Type
    bool m_IsActive;                      // 0x11 set when a stage is entered
    nw::snd::SoundHandle m_Handles[2];    // 0x14 the sea, the wind on the cliff
    u32 m_SoundId;                        // 0x1C
    const nn::math::VEC3* m_pListenerPos; // 0x20
    s8 m_OceanSide;                       // 0x24 0 left, 1 right
};
ASSERT_OFFSET(SoundSeaMgr, m_Type, 0x10);
ASSERT_OFFSET(SoundSeaMgr, m_Handles, 0x14);
ASSERT_OFFSET(SoundSeaMgr, m_SoundId, 0x1C);
ASSERT_OFFSET(SoundSeaMgr, m_pListenerPos, 0x20);
ASSERT_OFFSET(SoundSeaMgr, m_OceanSide, 0x24);
ASSERT_SIZE(SoundSeaMgr, 0x28);
