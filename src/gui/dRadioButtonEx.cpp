#include "gui/dRadioButtonEx.h"

#include "Other/dController.h"

namespace gui {
// 0x004FAD4C
void RadioButtonEx::Select(bool playAnimation)
{
    m_IsSelectedByCode = true;
    for (Widget::List::iterator it = s_List.begin(); it != s_List.end(); ++it) {
        Widget* widget = Widget::List::GetPointerFromNode(it.GetNode());
        if (widget->GetKind() != GetKind() || widget == this) {
            continue;
        }
        RadioButtonEx* other = static_cast<RadioButtonEx*>(widget);
        if (other->m_Desc2.group != m_Desc2.group || other->m_State != STATE_LOCKED) {
            continue;
        }
        if (playAnimation) {
            other->PlayAnimation(ANIM_RELEASE);
        } else {
            other->ApplyAnimationLimit(ANIM_PRESS, true);
        }
        other->m_State = STATE_IDLE;
    }
    if (playAnimation) {
        PlayAnimation(ANIM_DECIDE);
        m_State = STATE_DECIDED;
        return;
    }
    ApplyAnimationLimit(ANIM_DECIDE, false);
    m_IsHolding = false;
    m_State = STATE_LOCKED;
    m_IsSelectedByCode = false;
}

// 0x004FAE54 slot 0x30
void RadioButtonEx::UpdateDecided()
{
    if (IsAnimationDone(ANIM_DECIDE)) {
        m_IsHolding = false;
        m_State = STATE_LOCKED;
        m_IsSelectedByCode = false;
    }
}

// 0x004FAE84 slot 0x2C
void RadioButtonEx::UpdatePressed()
{
    if (sys::TouchGroup(m_Desc2.pLayout, m_GroupName) != 0) {
        if (!Controller::IsRelease(Controller::TOUCH)) {
            return;
        }
        Select(true);
        m_IsSelectedByCode = false;
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
        PlayAnimation(ANIM_DECIDE);
        if (m_Desc2.sounds[SOUND_DECIDE] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc2.sounds[SOUND_DECIDE]), m_Desc2.pLayout->m_SoundPan);
        }
        m_State = STATE_DECIDED;
        SendEvent(this, EVENT_DECIDE, 0);
    } else {
        PlayAnimation(ANIM_SLIDE_OUT);
        if (m_Desc2.sounds[SOUND_SLIDE_OUT] > 0) {
            SoundMgr::PlaySound(static_cast<SeID>(m_Desc2.sounds[SOUND_SLIDE_OUT]), m_Desc2.pLayout->m_SoundPan);
        }
        m_State = STATE_SLID_OUT;
    }
}

// 0x004FAFD4 slot 0x00
// 0x004FAFA4 (deleting dtor)
RadioButtonEx::~RadioButtonEx()
{
    // (only the base is destroyed)
}
} // namespace gui
