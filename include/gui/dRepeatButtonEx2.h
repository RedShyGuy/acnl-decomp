#pragma once

#include "decomp.h"
#include "gui/dRepeatButtonEx.h"

namespace gui {
// A RepeatButtonEx that is not pressed by its keys while the touch is held, and whose Reset plays the slide out.
// Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui15RepeatButtonEx2E @ 0x008D0E78
// vtable 0x00903FC8 (vptr 0x00903FD0), offset_to_top 0, 15 entries
class RepeatButtonEx2 : public ::gui::RepeatButtonEx
{
public:
    // (inline)
    explicit RepeatButtonEx2(const Description& desc) : RepeatButtonEx(desc) {}
    ~RepeatButtonEx2() override; // 0x004FE14C slot 0x00
    // 0x004FE11C slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_REPEAT_BUTTON_EX2; } // 0x00747824 slot 0x0C
    void Reset(bool isRelease) override; // 0x004FE078 slot 0x1C
    void UpdateIdle() override; // 0x004FDD6C slot 0x28
    void UpdatePressed() override; // 0x004FDE58 slot 0x2C
};
ASSERT_SIZE(RepeatButtonEx2, 0x230);
} // namespace gui
