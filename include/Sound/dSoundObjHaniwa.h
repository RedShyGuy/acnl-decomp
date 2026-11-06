#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

class SoundHaniwaMgr;

// RTTI 14SoundObjHaniwa @ 0x008CBDB4
// vtable 0x008F07C0 (vptr 0x008F07C8), offset_to_top 0, 14 entries
// A gyroid: its sequence (SE_HNW_*) plays in time with the music of SoundHaniwaMgr; the sequence tells with its variables how
// the gyroid moves, the object turns that into a phase of the dance (member and method names are ours).
class SoundObjHaniwa : public ::SoundObj<2>
{
public:
    // m_DanceState
    enum DanceState
    {
        DANCE_NONE = 0,
        DANCE_PREPARING = 1,
        DANCE_STARTING = 2,
        DANCE_DANCING = 3,
    };
    enum
    {
        KIND_NUM = 131,
        LEVEL_NUM = 4,
        GROUP_NUM = 4,
    };

    SoundObjHaniwa(); // 0x00278108
    // 0x002781D8
    // 0x002781C8 (deleting dtor)
    DECOMP_NOINLINE virtual ~SoundObjHaniwa();
    virtual void Stop() override; // 0x00277E00 slot 0x0C

    void Start(u32 kind, const nn::math::VEC3* world, const nn::math::VEC3* relative); // 0x00277254
    void StopDance(); // 0x0027728C
    void ApplyTempo(); // 0x00277298
    // the phase of the dance (-1: none)
    f32 UpdateDance(const nn::math::VEC3* world, const nn::math::VEC3* relative); // 0x0027731C
    void Register(); // 0x002777F8
    void SetLevel(u8 level, u32 index); // 0x0027787C
    DECOMP_NOINLINE bool ReadVariables(); // 0x00277984
    DECOMP_NOINLINE f32 CalcPhase(); // 0x00277B6C
    // the frames until the dance starts (-1: not known)
    s32 GetFramesToStart(); // 0x00277CE4
    void StartDance(bool isFree); // 0x00277EFC
    void StopSound(); // 0x00278094

    static const s16 s_LevelNotes[GROUP_NUM][LEVEL_NUM];

    u32 m_SoundId;           // 0x74
    u32 m_Index;             // 0x78 in SoundHaniwaMgr
    SoundHaniwaMgr* m_pMgr;  // 0x7C
    s32 m_DanceState;        // 0x80 DanceState
    bool m_IsFree;           // 0x84 the variation is random
    s16 m_Beat;              // 0x86 variable 9
    s16 m_BeatsFallback;     // 0x88 variable 7
    s16 m_Step;              // 0x8A written as variable 11
    s16 m_Beats;             // 0x8C variable 12
    s16 m_StartBeat;         // 0x8E variable 8
    s16 m_Variation;         // 0x90
    f32 m_Speed;             // 0x94 the phase is scaled down at the start
    bool m_IsStopRequested;  // 0x98
    f32 m_LastPhase;         // 0x9C
    s16 m_Mode;              // 0xA0 variable 0
    s16 m_Beats3;            // 0xA2 variable 5
    u16 m_StepMask;          // 0xA4 variables 1 to 4, a nibble each
    s16 m_Group;             // 0xA6 variable 6
    s16 m_Note;              // 0xA8
};
ASSERT_OFFSET(SoundObjHaniwa, m_SoundId, 0x74);
ASSERT_OFFSET(SoundObjHaniwa, m_DanceState, 0x80);
ASSERT_OFFSET(SoundObjHaniwa, m_Speed, 0x94);
ASSERT_OFFSET(SoundObjHaniwa, m_Note, 0xA8);
ASSERT_SIZE(SoundObjHaniwa, 0xAC);
