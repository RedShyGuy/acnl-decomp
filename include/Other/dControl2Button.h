#pragma once

#include "decomp.h"
#include "Other/dControlButtonBase.h"

namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// The control of 2 buttons of ComButton2Lyt (class name from the RTTI, all other names are ours): the buttons are members.
// RTTI 14Control2Button @ 0x008CBCAC
// vtable 0x008F02AC (vptr 0x008F02B4), offset_to_top 0, 15 entries
// vtable 0x008F02F0 (vptr 0x008F02F8), offset_to_top -228, 3 entries
class Control2Button : public ::ControlButtonBase
{
public:
    // (inline in the owners)
    Control2Button() : ControlButtonBase(m_Buttons, 2) {}
    // 0x00775FCC (thunk)
    // 0x00775F88 (deleting thunk)
    // 0x00268F38 (deleting dtor)
    ~Control2Button() override; // 0x00268F80 slot 0x00

    // the ok sounds of the buttons
    void SetSeOk(SeID a, SeID b); // 0x00268D28
    // decide (A) or cancel (B) buttons
    void SetDecide(bool a, bool b); // 0x00268D84
    // the sounds of the buttons
    void SetSe(SeID a, SeID b); // 0x00268DD4
    // the pad bits of the buttons
    void SetInput(u32 a, u32 b); // 0x00268E34
    // the texts of the buttons (message indices of label)
    void SetText(s32 a, s32 b, const char* label); // 0x00268E88
    // the panes the buttons are shown on
    void SetPanes(nw::lyt::Pane* a, nw::lyt::Pane* b); // 0x00268EE4

    CommonButtonSingle m_Buttons[2]; // 0x118
};
ASSERT_SIZE(Control2Button, 0x118 + 2 * 0x518);
