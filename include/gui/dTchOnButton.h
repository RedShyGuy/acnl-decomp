#pragma once

#include "decomp.h"
#include "gui/dButtonEx.h"

namespace gui {
// A ButtonEx that can turn itself off after a decision (SetTouchOn: disabled with its animation); the base of the
// text fields (EditArea), the keys that put a character (InputButton) and TchOnButtonSp. The binary has no vtable of
// its own for the class (its slots are only in the derived vtables). Only the class name is from the binary (RTTI);
// member and function names are ours.
// RTTI N3gui11TchOnButtonE @ 0x008D0E0C
class TchOnButton : public ::gui::ButtonEx
{
public:
    // (inline)
    explicit TchOnButton(const Description& desc) : ButtonEx(desc), m_IsTouchOn(false) {}
    // (the enabled state clears the touch-on state)
    void SetEnabled(bool enabled, bool playAnimation, bool keepAnimation) override; // 0x004FF650 slot 0x10 (falls into ButtonEx::SetEnabled)
    void UpdateDecided() override; // 0x004F7F5C slot 0x30
    // touch on: the button is disabled with its animation (not while held; never while disabled)
    virtual void SetTouchOn(bool isTouchOn); // 0x004F7F1C slot 0x3C
    virtual bool IsTouchOn() const { return m_IsTouchOn; } // 0x00747560 slot 0x40

    bool m_IsTouchOn;  // 0x119 (in the tail padding of ButtonEx) the button turned itself off after a decision
};
ASSERT_OFFSET(TchOnButton, m_IsTouchOn, 0x119);
ASSERT_SIZE(TchOnButton, 0x11C);
} // namespace gui
