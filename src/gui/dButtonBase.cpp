#include "gui/dButtonBase.h"

#include "lyt/dObject.h"

namespace gui {
// 0x004F3A64
ButtonBase::ButtonBase() : m_State(STATE_IDLE), m_IsActive(true), m_pDesc(NULL), m_GroupName()
{
}

// 0x004F3AF8 slot 0x00
// 0x004F3AC8 (deleting dtor)
ButtonBase::~ButtonBase()
{
    // (only the base is destroyed)
}

// 0x004F372C slot 0x14
void ButtonBase::SetVisible(bool visible, bool playAnimation)
{
    lyt::Object* layout = m_pDesc->pLayout;
    if (layout->m_ParentPaneName[0] != '\0') {
        layout->SetEnabled(visible);
        SetEnabled(visible, playAnimation, false);
    }
}

// 0x004F379C
void ButtonBase::StopAnimations()
{
    for (s32 i = 0; i < 5; i++) {
        lyt::AnmObject* anim = m_pDesc->anims[i];
        if (anim != NULL) {
            detail::StopIfPlaying(anim);
        }
    }
}

// 0x004F3888 slot 0x28
void ButtonBase::UpdateIdle()
{
    // empty (in the original too)
}

// 0x004F388C slot 0x30
void ButtonBase::UpdateDecided()
{
    // empty (in the original too)
}

// 0x004F3890 slot 0x38
void ButtonBase::UpdateDisabled()
{
    // empty (in the original too)
}

// 0x004F3894 slot 0x2C
void ButtonBase::UpdatePressed()
{
    // empty (in the original too)
}

// 0x004F3898 slot 0x34
void ButtonBase::UpdateSlidOut()
{
    // empty (in the original too)
}

// 0x004F389C slot 0x1C
void ButtonBase::Reset(bool isRelease)
{
    if (m_State == STATE_DISABLED) {
        return;
    }
    StopAnimations();
    if (isRelease) {
        lyt::AnmObject* anim = m_pDesc->anims[ANIM_RELEASE];
        if (anim != NULL) {
            detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
        }
    } else {
        lyt::AnmObject* anim = m_pDesc->anims[ANIM_PRESS];
        if (anim != NULL) {
            detail::ApplyLimit(anim, true);
        }
    }
    m_State = STATE_IDLE;
}

// 0x004F393C slot 0x18
void ButtonBase::SetActive(bool active)
{
    m_IsActive = active;
    m_IsHolding = false;
}

// 0x004F394C slot 0x10
void ButtonBase::SetEnabled(bool enabled, bool playAnimation, bool keepAnimation)
{
    if (enabled) {
        if (m_State != STATE_LOCKED) {
            if (!keepAnimation) {
                StopAnimations();
                lyt::AnmObject* anim = m_pDesc->anims[ANIM_PRESS];
                if (anim != NULL) {
                    detail::ApplyLimit(anim, true);
                }
            }
            m_State = STATE_IDLE;
        }
    } else {
        if (!keepAnimation) {
            if (playAnimation) {
                StopAnimations();
                lyt::AnmObject* anim = m_pDesc->anims[ANIM_DISABLE];
                if (anim != NULL) {
                    detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
                }
            } else if (m_State != STATE_LOCKED) {
                StopAnimations();
                lyt::AnmObject* anim = m_pDesc->anims[ANIM_PRESS];
                if (anim != NULL) {
                    detail::ApplyLimit(anim, true);
                }
            }
        }
        if (m_State != STATE_LOCKED) {
            m_State = STATE_DISABLED;
        }
    }
    m_IsHolding = false;
}
} // namespace gui
