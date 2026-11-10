#include "gui/dRepeatButton.h"

#include "Bs/dBsMenuMgr.h"
#include "Other/dController.h"
#include "Sound/dSoundMgr.h"
#include "lyt/dObject.h"
#include "sys/sys_Extern.h"

namespace gui {
namespace {
// (inline)
bool IsTouched(RepeatButton* button)
{
    return sys::TouchGroup(button->m_Desc.pLayout, button->m_GroupName) != 0;
}

// (inline)
void PlaySound(RepeatButton* button, s32 sound, f32 pan)
{
    if (button->m_Desc.sounds[sound] > 0) {
        SoundMgr::PlaySound(static_cast<SeID>(button->m_Desc.sounds[sound]), pan);
    }
}
} // namespace

// 0x00AD764C
s32 RepeatButton::s_DefaultSounds[SOUND_NUM] = {-1, -1, -1, -1, -1};

inline void RepeatButton::Decide()
{
    SendEvent(this, EVENT_DECIDE, 0);
    f32 pan = m_Desc.pLayout->m_SoundPan;
    if (m_Desc.isCenterSound == true) {
        pan = 0.0f;
    }
    PlaySound(this, SOUND_DECIDE, pan);
    if (m_Desc.decideSound > 0) {
        SoundMgr::SPlaySystemSoundChecked(m_Desc.decideSound, m_Desc.soundValue, m_Desc.pLayout->m_SoundPan);
    }
}

inline void RepeatButton::Repeat()
{
    m_HoldFrames++;
    if (m_HoldFrames < m_Desc.repeatDelay || (m_HoldFrames - m_Desc.repeatDelay) % m_Desc.repeatInterval != 0) {
        return;
    }
    SendEvent(this, EVENT_DECIDE, 0);
    if (m_Desc.repeatSound > 0) {
        SoundMgr::SPlaySystemSoundChecked(m_Desc.repeatSound, m_Desc.soundValue, m_Desc.pLayout->m_SoundPan);
    }
}

inline void RepeatButton::SlideOut()
{
    m_HoldFrames = 0;
    m_IsKeyPress = false;
    m_IsHolding = false;
    g_IsWidgetHolding = false;
    lyt::AnmObject* anim = m_Desc.anims[ANIM_SLIDE_OUT];
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    } else {
        anim = m_Desc.anims[ANIM_PRESS];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
        }
    }
    PlaySound(this, SOUND_SLIDE_OUT, 0.0f);
    m_State = STATE_SLID_OUT;
}

// 0x004F9F14
void RepeatButton::PlayRepeatAnimation()
{
    lyt::AnmObject* anim = m_Desc.repeatAnim;
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    }
}

// 0x004F9F50 slot 0x28
void RepeatButton::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (Controller::IsTrig(Controller::TOUCH)) {
        lyt::Object* layout = m_Desc.pLayout;
        if (layout == NULL || !layout->m_IsEnabled || !IsTouched(this)) {
            return;
        }
    } else {
        if (!BsMenuMgr::s_pInstance->IsPressed(m_Desc.keys) && (IsDirectionPressed() & m_Desc.keys) == 0) {
            return;
        }
        m_IsKeyPress = true;
        Decide();
    }
    lyt::AnmObject* anim = m_Desc.anims[ANIM_PRESS];
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    }
    PlaySound(this, SOUND_PRESS, m_Desc.pLayout->m_SoundPan);
    m_HoldFrames = 0;
    m_IsHolding = true;
    g_IsWidgetHolding = true;
    m_State = STATE_PRESSED;
}

// 0x004FA0EC slot 0x38
void RepeatButton::UpdateDisabled()
{
    if (Controller::IsTrig(Controller::TOUCH)) {
        lyt::Object* layout = m_Desc.pLayout;
        if (layout == NULL || !layout->m_IsEnabled || !IsTouched(this)) {
            return;
        }
    } else if (!BsMenuMgr::s_pInstance->IsPressed(m_Desc.keys) && (IsDirectionPressed() & m_Desc.keys) == 0) {
        return;
    }
    PlaySound(this, SOUND_DISABLED, m_Desc.pLayout->m_SoundPan);
}

// 0x004FA194 slot 0x2C
void RepeatButton::UpdatePressed()
{
    lyt::AnmObject* pressAnim = m_Desc.anims[ANIM_PRESS];
    if (pressAnim == NULL || pressAnim->m_State == lyt::AnmObject::STATE_STOPPING) {
        lyt::AnmObject* anim;
        if (m_Desc.isPlayRepeatAnim && (anim = m_Desc.repeatAnim) != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_LOOP);
        }
    }
    if (Controller::IsHold(Controller::TOUCH)) {
        if (m_Desc.pLayout == NULL) {
            return;
        }
        if (IsTouched(this)) {
            Repeat();
        } else {
            SlideOut();
        }
        return;
    }
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    if (mgr->IsHold(m_Desc.keys) || (IsDirectionHold() & m_Desc.keys) != 0) {
        Repeat();
        return;
    }
    if (BsMenuMgr::s_pInstance->IsRelease(m_Desc.keys)) {
        SlideOut();
        return;
    }
    // released on the button
    if (m_IsKeyPress) {
        m_IsKeyPress = false;
    } else if (m_HoldFrames < m_Desc.repeatDelay) {
        Decide();
    }
    lyt::AnmObject* anim = m_Desc.anims[ANIM_DECIDE];
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    } else {
        anim = m_Desc.anims[ANIM_PRESS];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
        }
    }
    m_State = STATE_IDLE;
}

// 0x004FA59C slot 0x34
void RepeatButton::UpdateSlidOut()
{
    if (!Controller::IsHold(Controller::TOUCH)) {
        m_IsKeyPress = false;
        m_State = STATE_IDLE;
        return;
    }
    if (m_Desc.pLayout == NULL || !IsTouched(this)) {
        return;
    }
    lyt::AnmObject* anim = m_Desc.anims[ANIM_PRESS];
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    }
    PlaySound(this, SOUND_PRESS_AGAIN, 0.0f);
    m_State = STATE_PRESSED;
}

// 0x004FA624
RepeatButton::Description::Description()
{
    pLayout = NULL;
    groupName = NULL;
    for (s32 i = 0; i < ANIM_NUM; i++) {
        anims[i] = NULL;
    }
    for (s32 i = 0; i < SOUND_NUM; i++) {
        sounds[i] = SOUND_DEFAULT;
    }
    m_Unknown0x30 = -1;
    keys = 0;
    repeatAnim = NULL;
    repeatDelay = 10;
    repeatInterval = 3;
    isPlayRepeatAnim = true;
    repeatSound = -1;
    decideSound = -1;
    isCenterSound = false;
    soundValue = 0;
}

// 0x004FA69C slot 0x08
void RepeatButton::Update()
{
    if (!m_IsActive) {
        return;
    }
    // another widget has the touch
    if (!m_IsHolding && g_IsWidgetHolding) {
        return;
    }
    switch (m_State) {
    case STATE_IDLE:
        UpdateIdle();
        break;
    case STATE_PRESSED:
        UpdatePressed();
        break;
    case STATE_SLID_OUT:
        UpdateSlidOut();
        break;
    case STATE_DISABLED:
        UpdateDisabled();
        break;
    }
    // the repeat animation only plays while pressed
    if (m_State != STATE_PRESSED) {
        lyt::AnmObject* anim = m_Desc.repeatAnim;
        if (anim != NULL) {
            detail::StopIfPlaying(anim);
        }
    }
}

// 0x004FA784
RepeatButton::RepeatButton(const Description& desc) : m_Desc(desc), m_HoldFrames(0), m_IsKeyPress(false)
{
    SetDescription(&m_Desc, s_DefaultSounds);
}

// 0x004FA864 slot 0x00
// 0x004FA834 (deleting dtor)
RepeatButton::~RepeatButton()
{
    // (only the base is destroyed)
}
} // namespace gui
