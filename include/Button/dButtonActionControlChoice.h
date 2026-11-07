#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// A control for a choice: the pressed button stays the current one (the hand cursor is always there).
// RTTI 25ButtonActionControlChoice @ 0x008CD054
// vtable 0x008F7C80 (vptr 0x008F7C88), offset_to_top 0, 15 entries
class ButtonActionControlChoice : public ::ButtonActionControl
{
public:
    ButtonActionControlChoice(); // 0x0033F6C4 (name after the RTTI)
    virtual ~ButtonActionControlChoice(); // 0x0033F720 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x0033F6E8 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void Process(); // 0x0033F3A4 slot 0x10 (name after libgarden)
    virtual void ProcessOk(); // 0x0033F61C slot 0x14 (name after libgarden)
    virtual void CheckInputForNodeOk(); // 0x0033F100 slot 0x1C (name after libgarden)

    // (names are ours)
    bool m_PlayTouchSound;     // 0xE4 the touch began on the current button
    u8 m_IsTouchOnButton;      // 0xE5 the touch began on a button
};
ASSERT_SIZE(ButtonActionControlChoice, 0xE8);
