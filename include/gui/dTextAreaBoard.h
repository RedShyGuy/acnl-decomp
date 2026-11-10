#pragma once

#include "decomp.h"
#include "gui/dTextAreaEx.h"

namespace gui {
// The controls of the bulletin board (lyt::TextAreaBoard): it has no pages, the arrow keys and the upper touch area
// scroll the lines (and the page pane with them); the touch areas are inactive while the field is pressed. Only the
// class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui13TextAreaBoardE @ 0x008D0E54
// vtable 0x00903F34 (vptr 0x00903F3C), offset_to_top 0, 7 entries
class TextAreaBoard : public ::gui::TextAreaEx
{
public:
    TextAreaBoard(); // 0x004FB514
    ~TextAreaBoard() override; // 0x004FB564 slot 0x00
    // 0x004FB554 slot 0x04 (deleting dtor)
    void OnEvent(Widget* sender, s32 event) override; // 0x004FB248 slot 0x10
    u32 GetPageNum() override { return 0; } // 0x004FB240 slot 0x18
};
ASSERT_SIZE(TextAreaBoard, 0x34);
} // namespace gui
