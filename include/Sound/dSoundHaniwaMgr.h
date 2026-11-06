#pragma once

#include "decomp.h"

namespace sead { class Heap; }
class SoundIHaniwaControl;
class SoundObjHaniwa;
class SoundChordTracker;

// The gyroids: all gyroids of a stage dance to the music of a controller (the class name is from the RTTI of its disposer,
// everything else is ours).
class SoundHaniwaMgr
{
public:
    class SingletonDisposer_;

    enum
    {
        OBJ_NUM = 64,
        INVALID_INDEX = 65,
    };
    // m_ControlKind
    enum ControlKind
    {
        CONTROL_ROOM_MUSIC = 0,
        CONTROL_DJ = 1,
    };

    SoundHaniwaMgr();
    static SoundHaniwaMgr* createInstance(sead::Heap* heap); // 0x00276A28
    static void destroyInstance(); // 0x0012FA08

    void SetControl(u32 kind, SoundIHaniwaControl* control); // 0x00276978
    void StopAll(); // 0x002769C4
    void Unregister(u32 index); // 0x002769F8
    // how many sounds were playing in the last second decides how the gyroids dance
    void UpdateLevel(); // 0x00276B70
    void UpdateNothing(); // 0x00276AF4
    void Update(); // 0x00276CF8
    u32 Register(SoundObjHaniwa* obj); // 0x00276D78
    u32 GetDancingCount() const; // 0x00719AEC
    // (copies; the defaults hold when there is no music)
    DECOMP_EMIT_COPY s16 GetBeat(); // 0x00719AD0
    DECOMP_EMIT_COPY u32 GetRandom(); // 0x00719B40
    DECOMP_EMIT_COPY s32 GetTicksPerBeat(); // 0x00719B5C
    DECOMP_EMIT_COPY s32 GetFramesPerBeat(); // 0x00719B7C
    DECOMP_EMIT_COPY s32 GetFramesToBeat(); // 0x00719B9C
    DECOMP_EMIT_COPY s16 GetTickInBeat(); // 0x00719BBC

    static void (SoundHaniwaMgr::*const s_UpdateFuncs[2])();
    static const u8 s_IsPlayerCounted[18];
    // the frames the dance runs out while the gyroids stop
    static const s32 s_StopFrames;
    static SoundHaniwaMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];  // 0x00
    s32 m_ControlKind;                 // 0x10
    SoundObjHaniwa* m_pObjs[OBJ_NUM];  // 0x14
    u32 m_ObjNum;                      // 0x114
    SoundIHaniwaControl* m_pControl;   // 0x118
    SoundChordTracker* m_pChords;      // 0x11C of the room music player
    f32 m_BeatScale;                   // 0x120
    u8 m_Level;                        // 0x124
    u8 m_History[150];                 // 0x125
    u8 _1BB;                           // 0x1BB
    u32 m_HistoryIndex;                // 0x1BC
    s32 m_Wait;                        // 0x1C0
};
ASSERT_OFFSET(SoundHaniwaMgr, m_ObjNum, 0x114);
ASSERT_OFFSET(SoundHaniwaMgr, m_History, 0x125);
ASSERT_OFFSET(SoundHaniwaMgr, m_HistoryIndex, 0x1BC);
ASSERT_SIZE(SoundHaniwaMgr, 0x1C4);
