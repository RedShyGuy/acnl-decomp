#include "gui/dRepeatButtonEx2.h"

namespace gui {
// 0x004FDD6C slot 0x28
void RepeatButtonEx2::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!(Controller::IsTrig(Controller::TOUCH) && IsTouched2()) &&
        (Controller::IsHold(Controller::TOUCH) || !BsMenuMgr::s_pInstance->IsPressed(m_Desc2.repeatKeys))) {
        return;
    }
    Press();
}

// 0x004FDE58 slot 0x2C
void RepeatButtonEx2::UpdatePressed()
{
    UpdatePressedImpl();
}

// 0x004FE078 slot 0x1C
void RepeatButtonEx2::Reset(bool isRelease)
{
    if (m_State == STATE_DISABLED) {
        return;
    }
    StopAllAnimations();
    if (isRelease) {
        PlayAnimation(ANIM_SLIDE_OUT);
        PlayAnimation(ANIM_EX_6);
    } else {
        ApplyAnimationLimit(ANIM_PRESS, true);
        ApplyAnimationLimit(ANIM_EX_5, true);
    }
    m_State = STATE_IDLE;
}

// 0x004FE14C slot 0x00
// 0x004FE11C (deleting dtor)
RepeatButtonEx2::~RepeatButtonEx2()
{
    // (only the base is destroyed)
}
} // namespace gui
