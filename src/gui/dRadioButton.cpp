#include "gui/dRadioButton.h"

#include "Other/dController.h"
#include "Sound/dSoundMgr.h"
#include "lyt/dObject.h"
#include "sys/sys_Extern.h"

namespace gui {
namespace {
// (inline)
bool IsTouched(RadioButton* button)
{
    return sys::TouchGroup(button->m_Desc.pLayout, button->m_GroupName) != 0;
}

// (inline)
void PlaySound(RadioButton* button, s32 sound)
{
    if (button->m_Desc.sounds[sound] > 0) {
        SoundMgr::PlaySound(static_cast<SeID>(button->m_Desc.sounds[sound]), button->m_Desc.pLayout->m_SoundPan);
    }
}
} // namespace

// 0x00AD7638
s32 RadioButton::s_DefaultSounds[SOUND_NUM] = {-1, -1, -1, -1, -1};

// 0x004F7880
void RadioButton::Select(bool playAnimation)
{
    m_IsSelectedByCode = true;
    for (Widget::List::iterator it = s_List.begin(); it != s_List.end(); ++it) {
        Widget* widget = Widget::List::GetPointerFromNode(it.GetNode());
        if (widget->GetKind() != GetKind() || widget == this) {
            continue;
        }
        RadioButton* other = static_cast<RadioButton*>(widget);
        if (other->m_Desc.group != m_Desc.group || other->m_State != STATE_LOCKED) {
            continue;
        }
        lyt::AnmObject* anim;
        if (playAnimation && (anim = other->m_Desc.anims[ANIM_RELEASE]) != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        } else {
            anim = other->m_Desc.anims[ANIM_PRESS];
            if (anim != NULL) {
                detail::ApplyLimit(anim, true);
            }
        }
        other->m_State = STATE_IDLE;
    }
    lyt::AnmObject* anim = m_Desc.anims[ANIM_DECIDE];
    if (playAnimation) {
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        }
        m_State = STATE_DECIDED;
        return;
    }
    if (anim == NULL) {
        anim = m_Desc.anims[ANIM_PRESS];
    }
    if (anim != NULL) {
        detail::ApplyLimit(anim, false);
    }
    m_IsHolding = false;
    m_IsSelectedByCode = false;
    m_State = m_Desc.isRepeatable != 0 ? STATE_IDLE : STATE_LOCKED;
}

// 0x004F7A1C slot 0x28
void RadioButton::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (Controller::IsTrig(Controller::TOUCH) && m_Desc.pLayout->m_IsEnabled && IsTouched(this)) {
        lyt::AnmObject* anim = m_Desc.anims[ANIM_PRESS];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        }
        PlaySound(this, SOUND_PRESS);
        m_IsHolding = true;
        g_IsWidgetHolding = true;
        m_State = STATE_PRESSED;
    }
}

// 0x004F7AC4 slot 0x30
void RadioButton::UpdateDecided()
{
    lyt::AnmObject* anim = m_Desc.anims[ANIM_DECIDE];
    if (anim != NULL && anim->m_State != lyt::AnmObject::STATE_STOPPING && anim->m_State == lyt::AnmObject::STATE_PLAYING) {
        return;
    }
    if (!m_IsSelectedByCode) {
        SendEvent(this, EVENT_DECIDE, 0);
    }
    m_IsHolding = false;
    m_IsSelectedByCode = false;
    m_State = m_Desc.isRepeatable != 0 ? STATE_IDLE : STATE_LOCKED;
}

// 0x004F7B38 slot 0x38
void RadioButton::UpdateDisabled()
{
    if (Controller::IsTrig(Controller::TOUCH) && m_Desc.pLayout->m_IsEnabled && IsTouched(this)) {
        PlaySound(this, SOUND_DISABLED);
    }
}

// 0x004F7B90 slot 0x2C
void RadioButton::UpdatePressed()
{
    s32 state;
    if (IsTouched(this)) {
        if (!Controller::IsRelease(Controller::TOUCH)) {
            return;
        }
        Select(true);
        m_IsSelectedByCode = false;
        lyt::AnmObject* anim = m_Desc.anims[ANIM_DECIDE];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        }
        f32 pan = m_Desc.pLayout->m_SoundPan;
        if (m_Desc.isCenterSound == true) {
            pan = 0.0f;
        }
        if (m_Desc.sounds[SOUND_DECIDE] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc.sounds[SOUND_DECIDE]), pan);
        }
        state = STATE_DECIDED;
    } else {
        lyt::AnmObject* anim = m_Desc.anims[ANIM_SLIDE_OUT];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        } else {
            // the press is undone
            anim = m_Desc.anims[ANIM_PRESS];
            if (anim != NULL) {
                detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
            }
        }
        PlaySound(this, SOUND_SLIDE_OUT);
        state = STATE_SLID_OUT;
    }
    m_State = state;
}

// 0x004F7CB4 slot 0x34
void RadioButton::UpdateSlidOut()
{
    if (!Controller::IsHold(Controller::TOUCH)) {
        m_State = STATE_IDLE;
        return;
    }
    if (IsTouched(this)) {
        lyt::AnmObject* anim = m_Desc.anims[ANIM_PRESS];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        }
        PlaySound(this, SOUND_PRESS_AGAIN);
        m_State = STATE_PRESSED;
    }
}

// 0x004F7D30
RadioButton::Description::Description()
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
    group = 0;
    isRepeatable = 0;
    isCenterSound = false;
}

// 0x004F7D80 slot 0x08
void RadioButton::Update()
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
    case STATE_DECIDED:
        UpdateDecided();
        break;
    case STATE_SLID_OUT:
        UpdateSlidOut();
        break;
    case STATE_LOCKED:
        break;
    case STATE_DISABLED:
        UpdateDisabled();
        break;
    }
}

// 0x004F7E14
RadioButton::RadioButton(const Description& desc) : m_Desc(desc), m_IsSelectedByCode(false)
{
    SetDescription(&m_Desc, s_DefaultSounds);
}

// 0x004F7EF0 slot 0x00
// 0x004F7EC0 (deleting dtor)
RadioButton::~RadioButton()
{
    // (only the base is destroyed)
}
} // namespace gui
