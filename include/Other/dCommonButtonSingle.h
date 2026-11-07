#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// RTTI 18CommonButtonSingle @ 0x008CC820
// vtable 0x008F4684 (vptr 0x008F468C), offset_to_top 0, 16 entries
class CommonButtonSingle : public ::ButtonActionNode
{
public:
    CommonButtonSingle(); // ctor address unknown
    virtual ~CommonButtonSingle(); // 0x002E1108 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002E1078 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void FUN_002b72e0(); // 0x002E0E6C slot 0x10 (name after libgarden)
    virtual void PlayOkSound(); // 0x00721270 slot 0x38 (name after libgarden)

    // (names are ours; game code that is not decompiled yet, called by ControlButtonBase)
    void SetVisible(bool visible); // 0x002E0854
    void SetDisabled(bool disabled); // 0x002E09F4
    // updates the text and animates the layout of the button
    void AnimateLayout(); // 0x002E0E94

    u8 _unknownE0[0x518 - 0xE0];
};
ASSERT_SIZE(CommonButtonSingle, 0x518);
