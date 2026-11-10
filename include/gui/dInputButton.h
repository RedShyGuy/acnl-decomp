#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"

namespace gui {
// A key of the keyboards that puts its character: it decides when it is touched (the release only plays the
// decision animation and sound). Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui11InputButtonE @ 0x008D0DF4
// vtable 0x00903D7C (vptr 0x00903D84), offset_to_top 0, 17 entries
class InputButton : public ::gui::TchOnButton
{
public:
    // (inline)
    explicit InputButton(const Description& desc) : TchOnButton(desc), m_IsSilent(false) {}
    ~InputButton() override; // 0x004F7854 slot 0x00
    // 0x004F7824 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_INPUT_BUTTON; } // 0x00747548 slot 0x0C
    void UpdateIdle() override; // 0x004F7734 slot 0x28
    void UpdatePressed() override; // 0x004F77C0 slot 0x2C

    bool m_IsSilent;  // 0x11A no sound on the decision (only cleared in the binary)
};
ASSERT_OFFSET(InputButton, m_IsSilent, 0x11A);
ASSERT_SIZE(InputButton, 0x11C);
} // namespace gui
