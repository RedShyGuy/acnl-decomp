#include "gui/dTchOnButton.h"

namespace gui {
// 0x004F7F1C slot 0x3C
void TchOnButton::SetTouchOn(bool isTouchOn)
{
    if (m_State == STATE_DISABLED) {
        isTouchOn = false;
    }
    m_IsTouchOn = isTouchOn;
    if (isTouchOn && !m_IsHolding) {
        SetEnabled(false, true, false);
    }
}

// 0x004F7F5C slot 0x30
void TchOnButton::UpdateDecided()
{
    if (m_IsTouchOn) {
        SetEnabled(false, true, false);
        return;
    }
    if (IsAnimationDone(ANIM_DECIDE)) {
        m_State = STATE_IDLE;
    }
}

// 0x004FF650 slot 0x10
void TchOnButton::SetEnabled(bool enabled, bool playAnimation, bool /*keepAnimation*/)
{
    m_IsTouchOn = false;
    ButtonEx::SetEnabled(enabled, playAnimation, false);
}
} // namespace gui
