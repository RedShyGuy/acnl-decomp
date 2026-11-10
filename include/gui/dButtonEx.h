#pragma once

#include "decomp.h"
#include "Sound/dSoundMgr.h"
#include "gui/dButtonBase.h"
#include "lyt/dObject.h"
#include "sys/sys_Extern.h"

namespace gui {
// A button with more animations: besides the animation of each Anim, five more for the same moment (e.g. the
// panes of the key top), and three animations for the derived buttons. Only the class name is from the binary (RTTI);
// member and function names are ours.
// RTTI N3gui8ButtonExE @ 0x008D0ECC
// vtable 0x00904114 (vptr 0x0090411C), offset_to_top 0, 15 entries
class ButtonEx : public ::gui::ButtonBase
{
public:
    // the animations of GetAnimation beyond ANIM_DISABLE (used by the derived buttons)
    enum
    {
        ANIM_EX_5 = ANIM_NUM,
        ANIM_EX_6,
        ANIM_EX_7,
        ANIM_EX_NUM
    };
    static const s32 SUB_ANIM_NUM = 5;

    struct Description : ButtonBase::Description
    {
        Description(); // 0x004FF3B4

        lyt::AnmObject* subAnims[ANIM_EX_NUM][SUB_ANIM_NUM];     // 0x38 played with the animation of the same index
        lyt::AnmObject* exAnims[ANIM_EX_NUM - ANIM_NUM];         // 0xD8 ANIM_EX_5..ANIM_EX_7
        lyt::Object* pKeyLayout;                                 // 0xE4 the key layout (set by TextAreaEx, never read)
    };

    explicit ButtonEx(const Description& desc); // 0x004FFA10
    ~ButtonEx() override; // 0x004FFAEC slot 0x00
    // 0x004FFABC slot 0x04 (deleting dtor)
    void Update() override; // 0x004FF530 slot 0x08
    u32 GetKind() const override { return KIND_BUTTON_EX; } // 0x00747860 slot 0x0C
    void SetEnabled(bool enabled, bool playAnimation, bool keepAnimation) override; // 0x004FF65C slot 0x10
    void Reset(bool isRelease) override; // 0x004FF4A8 slot 0x1C
    void UpdateIdle() override; // 0x004FF0F0 slot 0x28
    void UpdatePressed() override; // 0x004FF290 slot 0x2C
    void UpdateDecided() override; // 0x004FF198 slot 0x30
    void UpdateSlidOut() override; // 0x004FF33C slot 0x34
    void UpdateDisabled() override; // 0x004FF1B8 slot 0x38

    // the animation of the index (NULL: none)
    DECOMP_NOINLINE lyt::AnmObject* GetAnimation(s32 idx); // 0x004FF5DC
    // the SUB_ANIM_NUM animations played with it (NULL: a wrong index)
    DECOMP_NOINLINE lyt::AnmObject** GetSubAnimations(s32 idx); // 0x004FF07C
    // the animation and its sub animations play no more
    DECOMP_NOINLINE bool IsAnimationDone(s32 idx); // 0x004FEFF0
    // the animation and its sub animations are set to their start (isStart) or end frame
    DECOMP_NOINLINE void ApplyAnimationLimit(s32 idx, bool isStart); // 0x004FF204
    // the animation and its sub animations are played (a state without them plays those of ANIM_PRESS backwards)
    DECOMP_NOINLINE void PlayAnimation(s32 idx); // 0x004FF810
    DECOMP_NOINLINE void StopAnimation(s32 idx); // 0x004FF988

    // the default sounds (SOUND_DEFAULT in a description; the static initializer 0x00785A28 sets them to -1)
    static s32 s_DefaultSounds[SOUND_NUM];

protected:
    // (inline) the sound is played with the pan of the touched pane
    void PlaySound(s32 sound)
    {
        if (m_Desc.sounds[sound] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc.sounds[sound]), m_Desc.pLayout->m_SoundPan);
        }
    }
    // (inline) the sound is played with the pan
    void PlaySound(s32 sound, f32 pan)
    {
        if (m_Desc.sounds[sound] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc.sounds[sound]), pan);
        }
    }
    // (inline) the touch is on the bounding panes of the group
    bool IsTouched() { return sys::TouchGroup(m_Desc.pLayout, m_GroupName) != 0; }
    // (inline) the animations of all states stop
    void StopAllAnimations()
    {
        for (s32 i = 0; i < ANIM_NUM; i++) {
            StopAnimation(i);
        }
    }

public:
    Description m_Desc;  // 0x030
    bool m_IsReset;      // 0x118 Reset(false) was called in STATE_IDLE (cleared in the other states)
    // (the derived classes use the tail padding: TchOnButton 0x119, InputButton 0x11A)
};
ASSERT_OFFSET(ButtonEx, m_IsReset, 0x118);
ASSERT_SIZE(ButtonEx, 0x11C);
} // namespace gui
