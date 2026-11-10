#include "gui/dButtonEx2.h"

#include "Other/dController.h"

namespace gui {
namespace {
// (inline) the sound of the second description
void PlaySound2(ButtonEx2* button, s32 sound)
{
    if (button->m_Desc2.sounds[sound] > 0) {
        SoundMgr::PlaySound(static_cast<SeID>(button->m_Desc2.sounds[sound]), button->m_Desc2.pLayout->m_SoundPan);
    }
}

// (inline)
bool IsTouched2(ButtonEx2* button)
{
    return sys::TouchGroup(button->m_Desc2.pLayout, button->m_GroupName) != 0;
}
} // namespace

// 0x00500864 slot 0x28
void ButtonEx2::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!m_IsReset2) {
        Reset(false);
        m_IsReset2 = true;
    }
    if (Controller::IsTrig(Controller::TOUCH) && IsTouched2(this)) {
        PlayAnimation(ANIM_PRESS);
        PlayAnimation(ANIM_EX_5);
        PlaySound2(this, SOUND_PRESS);
        m_IsHolding = true;
        g_IsWidgetHolding = true;
        m_State = STATE_PRESSED;
        m_IsSlidOut = false;
    }
}

// 0x0050091C slot 0x30
void ButtonEx2::UpdateDecided()
{
    if (IsAnimationDone(ANIM_DECIDE) && IsAnimationDone(ANIM_EX_6)) {
        m_State = STATE_IDLE;
    }
}

// 0x00500950 slot 0x2C
void ButtonEx2::UpdatePressed()
{
    if (Controller::IsHold(Controller::TOUCH)) {
        if (IsTouched2(this)) {
            // the pressed look
            ApplyAnimationLimit(ANIM_PRESS, false);
            ApplyAnimationLimit(ANIM_EX_5, false);
            if (m_IsSlidOut == true) {
                PlaySound2(this, SOUND_PRESS);
                m_IsSlidOut = false;
            }
        } else {
            ApplyAnimationLimit(ANIM_PRESS, true);
            ApplyAnimationLimit(ANIM_EX_5, true);
            m_IsSlidOut = true;
        }
    } else if (Controller::IsRelease(Controller::TOUCH)) {
        if (IsTouched2(this)) {
            PlayAnimation(ANIM_DECIDE);
            PlayAnimation(ANIM_EX_7);
            PlaySound2(this, SOUND_DECIDE);
            m_State = STATE_DECIDED;
            SendEvent(this, EVENT_DECIDE, 0);
        } else {
            ApplyAnimationLimit(ANIM_PRESS, true);
            ApplyAnimationLimit(ANIM_EX_5, true);
            PlaySound2(this, SOUND_SLIDE_OUT);
            m_State = STATE_IDLE;
        }
    }
}

// 0x00500ABC slot 0x1C
void ButtonEx2::Reset(bool isRelease)
{
    if (m_State == STATE_DISABLED) {
        return;
    }
    StopAllAnimations();
    if (isRelease) {
        PlayAnimation(ANIM_PRESS);
    } else {
        ApplyAnimationLimit(ANIM_PRESS, true);
    }
    m_State = STATE_IDLE;
}

// 0x00500B44 slot 0x10
void ButtonEx2::SetEnabled(bool enabled, bool playAnimation, bool /*keepAnimation*/)
{
    if (enabled) {
        if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            ApplyAnimationLimit(ANIM_PRESS, true);
            ApplyAnimationLimit(ANIM_EX_5, true);
            m_State = STATE_IDLE;
        }
    } else {
        if (playAnimation) {
            StopAllAnimations();
            ApplyAnimationLimit(ANIM_DISABLE, true);
        } else if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            ApplyAnimationLimit(ANIM_DISABLE, true);
        }
        if (m_State != STATE_LOCKED) {
            m_State = STATE_DISABLED;
        }
    }
    m_IsHolding = false;
}

// 0x004FF3B0 (falls into ButtonEx::Description())
ButtonEx2::Description::Description()
{
    // (only the base is constructed)
}

// 0x00500C94
ButtonEx2::ButtonEx2(const ButtonEx::Description& desc) : ButtonEx(desc), m_Desc2(desc), m_IsReset2(true), m_IsSlidOut(false)
{
    m_pDesc = &m_Desc2;
}

// 0x00500D0C slot 0x00
// 0x00500CDC (deleting dtor)
ButtonEx2::~ButtonEx2()
{
    // (only the base is destroyed)
}
} // namespace gui
