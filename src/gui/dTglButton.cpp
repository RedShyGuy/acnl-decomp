#include "gui/dTglButton.h"

#include "Other/dController.h"
#include "Sound/dSoundLabel.h"
#include "gui/dRadioButton.h"

namespace gui {
// 0x00500D38 slot 0x30
void TglButton::UpdateDecided()
{
    if (IsAnimationDone(ANIM_DECIDE)) {
        m_State = STATE_IDLE;
    }
}

// 0x00500D58 slot 0x2C
void TglButton::UpdatePressed()
{
    if (IsTouched()) {
        if (Controller::IsRelease(Controller::TOUCH)) {
            lyt::AnmObject* anim = GetAnimation(ANIM_PRESS);
            lyt::AnmObject** subAnims = GetSubAnimations(ANIM_PRESS);
            if (anim != NULL) {
                anim->Stop();
            }
            for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
                if (subAnims[i] != NULL) {
                    subAnims[i]->Stop();
                }
            }
            PlaySound(SOUND_DECIDE);
            m_IsOn = !m_IsOn;
            if (m_IsOn) {
                m_Desc = m_OnDesc;
            } else {
                m_Desc = m_OffDesc;
            }
            PlayAnimation(ANIM_DECIDE);
            m_State = STATE_DECIDED;
            SendEvent(this, EVENT_DECIDE, 0);
        }
    } else {
        PlayAnimation(ANIM_SLIDE_OUT);
        PlaySound(SOUND_SLIDE_OUT);
        m_State = STATE_SLID_OUT;
    }
}

// 0x00500EB4 slot 0x1C
void TglButton::Reset(bool isRelease)
{
    if (m_State == STATE_DISABLED) {
        return;
    }
    StopAllAnimations();
    if (isRelease) {
        PlayAnimation(ANIM_RELEASE);
    } else {
        ApplyAnimationLimit(ANIM_PRESS, true);
    }
    m_State = STATE_IDLE;
}

// 0x00500F3C
void TglButton::SetOn(bool isOn)
{
    StopAllAnimations();
    m_IsOn = isOn;
    if (isOn) {
        m_Desc = m_OnDesc;
    } else {
        m_Desc = m_OffDesc;
    }
    ApplyAnimationLimit(ANIM_PRESS, true);
    m_State = STATE_IDLE;
}

// 0x00500FD0 slot 0x10
void TglButton::SetEnabled(bool enabled, bool playAnimation, bool /*keepAnimation*/)
{
    if (enabled) {
        if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            m_IsOn = false;
            m_Desc = m_OffDesc;
            ApplyAnimationLimit(ANIM_PRESS, true);
            m_State = STATE_IDLE;
        }
    } else {
        if (playAnimation) {
            StopAllAnimations();
            m_IsOn = false;
            m_Desc = m_OffDesc;
            PlayAnimation(ANIM_DISABLE);
        } else if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            m_IsOn = false;
            m_Desc = m_OffDesc;
            ApplyAnimationLimit(ANIM_PRESS, true);
        }
        if (m_State != STATE_LOCKED) {
            m_State = STATE_DISABLED;
        }
    }
    m_IsHolding = false;
}

// 0x00501154
TglButton::TglButton(const Description& desc) : ButtonEx(desc), m_IsOn(false)
{
    m_OnDesc = m_Desc;
    m_OffDesc = m_Desc;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < SUB_ANIM_NUM; j++) {
            m_OnDesc.subAnims[i][j] = desc.onSubAnims[i][j];
        }
    }
    // the decision sounds the switch it makes
    if (desc.sounds[SOUND_PRESS] == SOUND_DEFAULT) {
        m_OnDesc.sounds[SOUND_PRESS] = SE_SYS_SWK_KEY;
        m_OffDesc.sounds[SOUND_PRESS] = SE_SYS_SWK_KEY;
    }
    if (desc.sounds[SOUND_PRESS_AGAIN] == SOUND_DEFAULT) {
        m_OnDesc.sounds[SOUND_PRESS_AGAIN] = RadioButton::s_DefaultSounds[SOUND_PRESS_AGAIN];
        m_OffDesc.sounds[SOUND_PRESS_AGAIN] = RadioButton::s_DefaultSounds[SOUND_PRESS_AGAIN];
    }
    if (desc.sounds[SOUND_SLIDE_OUT] == SOUND_DEFAULT) {
        m_OnDesc.sounds[SOUND_SLIDE_OUT] = RadioButton::s_DefaultSounds[SOUND_SLIDE_OUT];
        m_OffDesc.sounds[SOUND_SLIDE_OUT] = RadioButton::s_DefaultSounds[SOUND_SLIDE_OUT];
    }
    if (desc.sounds[SOUND_DECIDE] == SOUND_DEFAULT) {
        m_OffDesc.sounds[SOUND_DECIDE] = SE_SYS_SWK_TOGGLE_ON;
        m_OnDesc.sounds[SOUND_DECIDE] = SE_SYS_SWK_TOGGLE_OFF;
    }
    if (desc.sounds[SOUND_DISABLED] == SOUND_DEFAULT) {
        m_OnDesc.sounds[SOUND_DISABLED] = RadioButton::s_DefaultSounds[SOUND_DISABLED];
        m_OffDesc.sounds[SOUND_DISABLED] = RadioButton::s_DefaultSounds[SOUND_DISABLED];
    }
}

// 0x005012EC slot 0x00
// 0x005012BC (deleting dtor)
TglButton::~TglButton()
{
    // (only the base is destroyed)
}
} // namespace gui
