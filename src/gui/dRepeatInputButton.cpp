#include "gui/dRepeatInputButton.h"

namespace gui {
namespace {
// (inline)
void PlaySound3(RepeatInputButton* button, s32 sound)
{
    if (button->m_Desc3.sounds[sound] > 0) {
        SoundMgr::PlaySound(static_cast<SeID>(button->m_Desc3.sounds[sound]), 0.0f);
    }
}
} // namespace

// 0x004FE178 slot 0x28
void RepeatInputButton::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!Controller::IsTrig(Controller::TOUCH) || sys::TouchGroup(m_Desc3.pLayout, m_GroupName) == 0) {
        return;
    }
    m_IsRepeatStopped = false;
    if (m_Desc3.isDecideOnPress) {
        SendEvent(this, EVENT_DECIDE, 0);
    }
    PlayAnimation(ANIM_PRESS);
    m_HoldFrames = 0;
    m_Stage = 0;
    m_StageRepeatNum = 0;
    m_IsHolding = true;
    g_IsWidgetHolding = true;
    m_State = STATE_PRESSED;
}

// 0x004FE214 slot 0x2C
void RepeatInputButton::UpdatePressed()
{
    if (!Controller::IsHold(Controller::TOUCH)) {
        // released: a short press decides
        if (!m_IsRepeatStopped && m_HoldFrames < m_Desc3.repeatDelay) {
            if (m_Desc3.isDecideOnPress) {
                PlaySound3(this, SOUND_DECIDE);
            } else {
                SendEvent(this, EVENT_DECIDE, 0);
            }
        }
        PlayAnimation(ANIM_SLIDE_OUT);
        m_State = STATE_IDLE;
        return;
    }
    if (sys::TouchGroup(m_Desc3.pLayout, m_GroupName) == 0) {
        m_HoldFrames = 0;
        m_Stage = 0;
        m_StageRepeatNum = 0;
        PlayAnimation(ANIM_SLIDE_OUT);
        PlaySound3(this, SOUND_SLIDE_OUT);
        m_State = STATE_SLID_OUT;
        return;
    }
    if (m_IsRepeatStopped) {
        return;
    }
    m_HoldFrames++;
    if (m_HoldFrames < m_Desc3.repeatDelay || (m_HoldFrames - m_Desc3.repeatDelay) % m_Desc3.repeatIntervals[m_Stage] != 0) {
        return;
    }
    SendEvent(this, EVENT_DECIDE, m_Stage + 1);
    m_HoldFrames = m_Desc3.repeatDelay;
    if (m_Stage < STAGE_NUM - 1 && m_Desc3.stageRepeats[m_Stage] != -1) {
        m_StageRepeatNum++;
        if (m_StageRepeatNum >= m_Desc3.stageRepeats[m_Stage]) {
            m_Stage++;
            m_StageRepeatNum = 0;
        }
    }
}

// 0x004FE3B8
RepeatInputButton::RepeatInputButton(const Description& desc) : RepeatButtonEx(desc), m_Desc3(desc), m_IsRepeatStopped(false)
{
    static_cast<ButtonEx::Description&>(m_Desc3) = m_Desc;
    SetRepeatDefaultSounds(&m_Desc3, desc);
}

// 0x004FE518 slot 0x00
// 0x004FE4E8 (deleting dtor)
RepeatInputButton::~RepeatInputButton()
{
    // (only the base is destroyed)
}
} // namespace gui
