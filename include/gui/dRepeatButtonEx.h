#pragma once

#include "decomp.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dController.h"
#include "gui/dButtonEx.h"
#include "gui/dRepeatButton.h"

namespace gui {
// A ButtonEx that repeats while it is held (touch or its keys), in up to three stages with their own intervals (each
// repeat sends EVENT_DECIDE with the stage + 1); a short press decides when it is released. The animations
// ANIM_EX_5 (with the press) and ANIM_EX_6 (with the release) are played too. Only the class name is from the binary
// (RTTI); member and function names are ours.
// RTTI N3gui14RepeatButtonExE @ 0x008D0E60
// vtable 0x00903F58 (vptr 0x00903F60), offset_to_top 0, 15 entries
class RepeatButtonEx : public ::gui::ButtonEx
{
public:
    static const s32 STAGE_NUM = 3;

    struct Description : ButtonEx::Description
    {
        // (inline)
        Description() : isDecideOnPress(true), repeatDelay(15), repeatKeys(0)
        {
            for (s32 i = 0; i < STAGE_NUM; i++) {
                repeatIntervals[i] = 15;
            }
            for (s32 i = 0; i < STAGE_NUM - 1; i++) {
                stageRepeats[i] = -1;
            }
        }

        bool isDecideOnPress;               // 0xE8 the event is sent when pressed, not when released
        s32 repeatDelay;                    // 0xEC the frames before the first repeat
        s32 repeatIntervals[STAGE_NUM];     // 0xF0 the frames between the repeats of each stage
        s32 stageRepeats[STAGE_NUM - 1];    // 0xFC the repeats before the next stage (-1: the stage stays)
        u32 repeatKeys;                     // 0x104 the keys of BsMenuMgr that press the button
    };

    // (inline) the description; its sounds left at SOUND_DEFAULT get those of RepeatButton
    explicit RepeatButtonEx(const Description& desc) : ButtonEx(desc), m_HoldFrames(0), m_Stage(0), m_StageRepeatNum(0), m_Desc2(desc)
    {
        static_cast<ButtonEx::Description&>(m_Desc2) = m_Desc;
        SetRepeatDefaultSounds(&m_Desc2, desc);
    }
    ~RepeatButtonEx() override; // 0x004FB890 slot 0x00
    // 0x004FB860 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_REPEAT_BUTTON_EX; } // 0x00747590 slot 0x0C
    void UpdateIdle() override; // 0x004FB568 slot 0x28
    void UpdatePressed() override; // 0x004FB640 slot 0x2C

protected:
    // (inline)
    static void SetRepeatDefaultSounds(Description* dst, const Description& src);
    // (inline) the touch is on the bounding panes of the group
    bool IsTouched2() { return sys::TouchGroup(m_Desc2.pLayout, m_GroupName) != 0; }
    // (inline) the sound of the second description without pan
    void PlaySound2(s32 sound)
    {
        if (m_Desc2.sounds[sound] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc2.sounds[sound]), 0.0f);
        }
    }
    // (inline) the press of UpdateIdle (sent at once if isDecideOnPress)
    void Press()
    {
        if (m_Desc2.isDecideOnPress) {
            SendEvent(this, EVENT_DECIDE, 0);
        }
        PlayAnimation(ANIM_PRESS);
        PlayAnimation(ANIM_EX_5);
        PlaySound2(SOUND_PRESS);
        m_HoldFrames = 0;
        m_Stage = 0;
        m_StageRepeatNum = 0;
        m_IsHolding = true;
        g_IsWidgetHolding = true;
        m_State = STATE_PRESSED;
    }
    // (inline) one more frame held: the repeats and the stages
    void Repeat()
    {
        m_HoldFrames++;
        if (m_HoldFrames < m_Desc2.repeatDelay || (m_HoldFrames - m_Desc2.repeatDelay) % m_Desc2.repeatIntervals[m_Stage] != 0) {
            return;
        }
        SendEvent(this, EVENT_DECIDE, m_Stage + 1);
        PlaySound2(m_Desc2.isDecideOnPress ? SOUND_PRESS : SOUND_DECIDE);
        m_HoldFrames = m_Desc2.repeatDelay;
        if (m_Stage < STAGE_NUM - 1 && m_Desc2.stageRepeats[m_Stage] != -1) {
            m_StageRepeatNum++;
            if (m_StageRepeatNum >= m_Desc2.stageRepeats[m_Stage]) {
                m_Stage++;
                m_StageRepeatNum = 0;
            }
        }
    }
    // (inline) UpdatePressed (RepeatButtonEx2 has a copy)
    DECOMP_ALWAYS_INLINE void UpdatePressedImpl()
    {
        if (!Controller::IsHold(Controller::TOUCH) && !BsMenuMgr::s_pInstance->IsHold(m_Desc2.repeatKeys)) {
            // released: a short press decides
            if (m_HoldFrames < m_Desc2.repeatDelay) {
                if (!m_Desc2.isDecideOnPress) {
                    SendEvent(this, EVENT_DECIDE, 0);
                }
                PlaySound2(SOUND_DECIDE);
            }
            PlayAnimation(ANIM_SLIDE_OUT);
            PlayAnimation(ANIM_EX_6);
            m_State = STATE_IDLE;
            return;
        }
        if (IsTouched2() || BsMenuMgr::s_pInstance->IsHold(m_Desc2.repeatKeys)) {
            Repeat();
            return;
        }
        m_HoldFrames = 0;
        m_Stage = 0;
        m_StageRepeatNum = 0;
        PlayAnimation(ANIM_SLIDE_OUT);
        PlayAnimation(ANIM_EX_6);
        PlaySound2(SOUND_SLIDE_OUT);
        m_State = STATE_SLID_OUT;
    }

public:
    s32 m_HoldFrames;       // 0x11C
    s32 m_Stage;            // 0x120
    s32 m_StageRepeatNum;   // 0x124 the repeats in the stage so far
    Description m_Desc2;    // 0x128
};
ASSERT_SIZE(RepeatButtonEx::Description, 0x108);

inline void RepeatButtonEx::SetRepeatDefaultSounds(Description* dst, const Description& src)
{
    for (s32 i = 0; i < SOUND_NUM; i++) {
        if (src.sounds[i] == SOUND_DEFAULT) {
            dst->sounds[i] = RepeatButton::s_DefaultSounds[i];
        }
    }
}
ASSERT_SIZE(RepeatButtonEx, 0x230);
} // namespace gui
