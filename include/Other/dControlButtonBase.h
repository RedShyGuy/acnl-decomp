#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Other/dCommonButtonSingle.h"
#include "state/dMode.h"
#include "Sound/dSoundLabel.h"

// A control over an array of CommonButtonSingle with a state machine of its own (member and helper names are ours).
// RTTI 17ControlButtonBase @ 0x008CC528
// vtable 0x008F37D8 (vptr 0x008F37E0), offset_to_top 0, 15 entries
// vtable 0x008F381C (vptr 0x008F3824), offset_to_top -228, 3 entries
class ControlButtonBase : public ::ButtonActionControl, public ::state::Mode<ControlButtonBase>
{
public:
    ControlButtonBase(CommonButtonSingle* buttons, s32 buttonNum); // 0x002CDA10 (name after the RTTI)
    virtual ~ControlButtonBase(); // 0x002F8184 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x002CDA78 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    // the visible buttons, then the state
    virtual void Update(); // 0x002CD748 slot 0x08 | slot vf_0x08 of ButtonActionControl

    void AnimateButtons(); // 0x002CD810
    void SetButtonOkSound(SeID se, s32 index); // 0x002CD8AC
    void SetDisabled(bool disabled); // 0x002CD944
    // only the last num buttons are visible and active
    void SetVisibleNum(s32 num); // 0x002CD99C

    CommonButtonSingle* m_pButtons;  // 0x108
    s32 m_ButtonNum;                 // 0x10C
    s32 m_FirstVisible;              // 0x110
    u8 m_Unknown0x114;               // 0x114
};
ASSERT_OFFSET(ControlButtonBase, m_pButtons, 0x108);
ASSERT_SIZE(ControlButtonBase, 0x118);
