#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 17SoundObjFurniture @ 0x008CC6D0
// vtable 0x008F3D74 (vptr 0x008F3D7C), offset_to_top 0, 16 entries
// A piece of furniture: a sound when it is turned on and off, a loop while it is on, and sounds at frames of its animation.
// The sounds go through SoundFurnitureMgr (member and method names are ours).
class SoundObjFurniture : public ::SoundObj<2>
{
public:
    enum
    {
        KIND_NUM = 409,
        CUE_SET_NUM = 15,
        CUE_NUM = 10,
    };
    // Info::flags
    enum
    {
        FLAG_LOOP_ONLY_ON = 1 << 0, // the loop only plays while it is on
        FLAG_RESTART = 1 << 1,      // a sound that plays already starts again
    };
    struct Info
    {
        u32 soundOn;  // 0x0
        u32 soundOff; // 0x4
        u32 soundLoop; // 0x8
        u8 cueSet;    // 0xC index in s_Cues, 0: none
        u8 flags;     // 0xD
    };
    struct Cue
    {
        u16 frame;   // 0x0
        u32 soundId; // 0x4 0: the end of the cues
    };

    SoundObjFurniture(); // 0x002D3018
    // 0x002D315C
    // 0x002D30B0 (deleting dtor)
    virtual ~SoundObjFurniture();
    virtual void Stop() override; // 0x002D2DAC slot 0x0C
    virtual sead::SoundHandle* StartSound(u32 soundId) override; // 0x002D2C50 slot 0x1C
    virtual sead::SoundHandle* HoldSound(u32 soundId) override; // 0x002D29A0 slot 0x24
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 kind, bool isOn); // 0x002D250C slot 0x38
    // frame: of the animation
    virtual void Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame); // 0x002D2AAC slot 0x3C

    void TurnOff(); // 0x002D25DC
    // true if the loop plays
    DECOMP_EMIT_COPY bool HoldLoop() // 0x002D2614
    {
        u32 soundId = m_pInfo->soundLoop;
        if (soundId == 0) {
            return false;
        }
        if ((m_pInfo->flags & FLAG_LOOP_ONLY_ON) && !m_IsOn) {
            return false;
        }
        HoldSound(soundId);
        return true;
    }
    void UpdateCues(bool isLoop, f32 frame); // 0x002D265C
    // (the second argument is not used)
    void PlaySound(u32 soundId, bool isCue); // 0x002D2830
    void SetKind(u32 kind); // 0x002D2F6C
    // the off sound plays on one of the handles (a copy in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsOffSoundPlaying() // 0x0072050C
    {
        if (m_pInfo == NULL || m_pInfo->soundOff == 0) {
            return false;
        }
        return m_HandleSet.m_Handles[0].GetId() == m_pInfo->soundOff || m_HandleSet.m_Handles[1].GetId() == m_pInfo->soundOff;
    }
    void TurnOn(); // 0x002D2FE0

    static const Info s_Infos[KIND_NUM];
    static const Cue s_Cues[CUE_SET_NUM][CUE_NUM];

    u32 m_Index;          // 0x74 in SoundFurnitureMgr
    bool m_IsOn;          // 0x78
    bool m_IsCueDisabled; // 0x79
    u16 m_Kind;           // 0x7A
    const Info* m_pInfo;  // 0x7C
    const Cue* m_pCues;   // 0x80
    u32 m_LastCue;        // 0x84
    f32 m_LastFrame;      // 0x88
    bool m_IsHeld;        // 0x8C
    bool m_IsSwitch;      // 0x8D on/off follows m_NextIsOn
    bool m_NextIsOn;      // 0x8E
    bool m_IsAllowed;     // 0x8F the start of the furniture sounds is spread over the frames
};
ASSERT_OFFSET(SoundObjFurniture, m_Index, 0x74);
ASSERT_OFFSET(SoundObjFurniture, m_LastFrame, 0x88);
ASSERT_SIZE(SoundObjFurniture, 0x90);
