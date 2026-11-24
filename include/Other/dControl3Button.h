#pragma once

#include "decomp.h"
#include "Other/dControlButtonBase.h"

namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// The control of 3 buttons of ComButton3Lyt (class name from the RTTI, all other names are ours): the buttons are members.
// RTTI 14Control3Button @ 0x008CBCB8
// vtable 0x008F0304 (vptr 0x008F030C), offset_to_top 0, 15 entries
// vtable 0x008F0348 (vptr 0x008F0350), offset_to_top -228, 3 entries
class Control3Button : public ::ControlButtonBase
{
public:
    // (inline in the owners)
    Control3Button() : ControlButtonBase(m_Buttons, 3) {}
    // 0x00776050 (thunk)
    // 0x0077600C (deleting thunk)
    // 0x00269178 (deleting dtor)
    ~Control3Button() override; // 0x002691C0 slot 0x00

    // the ok sounds of the buttons
    void SetSeOk(SeID a, SeID b, SeID c); // 0x00268FC4
    // decide (A) or cancel (B) buttons
    void SetDecide(bool a, bool b, bool c); // 0x00269020
    // the pad bits of the buttons
    void SetInput(u32 a, u32 b, u32 c); // 0x00269074
    // the texts of the buttons (message indices of label)
    void SetText(s32 a, s32 b, s32 c, const char* label); // 0x002690C8
    // the panes the buttons are shown on
    void SetPanes(nw::lyt::Pane* a, nw::lyt::Pane* b, nw::lyt::Pane* c); // 0x00269124

    CommonButtonSingle m_Buttons[3]; // 0x118
};
ASSERT_SIZE(Control3Button, 0x118 + 3 * 0x518);
