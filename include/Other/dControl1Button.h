#pragma once

#include "decomp.h"
#include "Other/dControlButtonBase.h"

namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// The control of button of ComButton1Lyt (class name from the RTTI, all other names are ours): the buttons are members.
// RTTI 14Control1Button @ 0x008CBCA0
// vtable 0x008F0254 (vptr 0x008F025C), offset_to_top 0, 15 entries
// vtable 0x008F0298 (vptr 0x008F02A0), offset_to_top -228, 3 entries
class Control1Button : public ::ControlButtonBase
{
public:
    // (inline in the owners)
    Control1Button() : ControlButtonBase(m_Buttons, 1) {}
    // 0x00775F48 (thunk)
    // 0x00775F04 (deleting thunk)
    // 0x00268C9C (deleting dtor)
    ~Control1Button() override; // 0x00268CE4 slot 0x00

    // the ok sounds of the button
    void SetSeOk(SeID value); // 0x00268C7C
    // decide (A) or cancel (B) button
    void SetDecide(bool value); // 0x002E080C
    // the pad bits of the button
    void SetInput(u32 value); // 0x00268C90
    // the texts of the button (message indices of label)
    void SetText(s32 value, const char* label); // 0x002E08CC
    // the pane the button are shown on
    void SetPane(nw::lyt::Pane* value); // 0x002E0940

    CommonButtonSingle m_Buttons[1]; // 0x118
};
ASSERT_SIZE(Control1Button, 0x118 + 1 * 0x518);
