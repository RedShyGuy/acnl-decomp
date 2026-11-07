#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// A control with two more animations (only the game code uses them).
// RTTI 35ButtonActionControlTouchOkAnmSwitch @ 0x008CD148
// vtable 0x008F82CC (vptr 0x008F82D4), offset_to_top 0, 15 entries
class ButtonActionControlTouchOkAnmSwitch : public ::ButtonActionControl
{
public:
    ButtonActionControlTouchOkAnmSwitch(); // 0x004DC1D8 (name after the RTTI)
    virtual ~ButtonActionControlTouchOkAnmSwitch(); // 0x004DC22C slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x004DC200 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)

    // (names are ours)
    ssys::ma::lyt::Animation m_Animation0;  // 0x0E4
    ssys::ma::lyt::Animation m_Animation1;  // 0x10C
};
ASSERT_SIZE(ButtonActionControlTouchOkAnmSwitch, 0x134);
