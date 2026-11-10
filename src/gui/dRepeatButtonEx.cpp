#include "gui/dRepeatButtonEx.h"

namespace gui {
// 0x004FB568 slot 0x28
void RepeatButtonEx::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!(Controller::IsTrig(Controller::TOUCH) && IsTouched2()) && !BsMenuMgr::s_pInstance->IsPressed(m_Desc2.repeatKeys)) {
        return;
    }
    Press();
}

// 0x004FB640 slot 0x2C
void RepeatButtonEx::UpdatePressed()
{
    UpdatePressedImpl();
}

// 0x004FB890 slot 0x00
// 0x004FB860 (deleting dtor)
RepeatButtonEx::~RepeatButtonEx()
{
    // (only the base is destroyed)
}
} // namespace gui
