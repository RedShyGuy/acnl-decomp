#include "gui/dInputButton.h"

#include "Other/dController.h"

namespace gui {
// 0x004F7734 slot 0x28
void InputButton::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (Controller::IsTrig(Controller::TOUCH) && IsTouched()) {
        m_IsSilent = false;
        PlayAnimation(ANIM_PRESS);
        m_IsHolding = true;
        g_IsWidgetHolding = true;
        m_State = STATE_PRESSED;
        SendEvent(this, EVENT_DECIDE, 0);
    }
}

// 0x004F77C0 slot 0x2C
void InputButton::UpdatePressed()
{
    if (Controller::IsHold(Controller::TOUCH) && IsTouched()) {
        return;
    }
    if (!m_IsSilent) {
        PlaySound(SOUND_DECIDE);
    }
    PlayAnimation(ANIM_DECIDE);
    m_State = STATE_DECIDED;
}

// 0x004F7854 slot 0x00
// 0x004F7824 (deleting dtor)
InputButton::~InputButton()
{
    // (only the base is destroyed)
}
} // namespace gui
