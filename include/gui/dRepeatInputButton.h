#pragma once

#include "decomp.h"
#include "gui/dRepeatButtonEx.h"

namespace gui {
// The repeating key of the keyboards (e.g. the backspace): a RepeatButtonEx that is only touched, with a third copy of
// the description and no sounds of its own on the press. Only the class name is from the binary (RTTI); member and
// function names are ours.
// RTTI N3gui17RepeatInputButtonE @ 0x008D0E84
// vtable 0x0090400C (vptr 0x00904014), offset_to_top 0, 15 entries
class RepeatInputButton : public ::gui::RepeatButtonEx
{
public:
    explicit RepeatInputButton(const Description& desc); // 0x004FE3B8
    ~RepeatInputButton() override; // 0x004FE518 slot 0x00
    // 0x004FE4E8 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_REPEAT_INPUT_BUTTON; } // 0x00747830 slot 0x0C
    void UpdateIdle() override; // 0x004FE178 slot 0x28
    void UpdatePressed() override; // 0x004FE214 slot 0x2C

    Description m_Desc3;     // 0x230
    bool m_IsRepeatStopped;  // 0x338 no more repeats or decision until the next press (only cleared here)
};
ASSERT_SIZE(RepeatInputButton, 0x33C);
} // namespace gui
