#pragma once

#include "decomp.h"
#include "gui/dTextAreaEx.h"
#include "lyt/dTextAreaMail.h"

namespace gui {
// The controls of a letter (lyt::TextAreaMail, pages 0 header, 1..6 body, 7 signature): the arrow keys and the touch
// areas scroll the lines and go from page to page; leaving the body for the signature keeps the page and the first
// line, coming back restores them and the text of the body. Only the class name is from the binary (RTTI); member and
// function names are ours.
// RTTI N3gui12TextAreaMailE @ 0x008D0E30
// vtable 0x00903E80 (vptr 0x00903E88), offset_to_top 0, 7 entries
class TextAreaMail : public ::gui::TextAreaEx
{
public:
    // the pages of the letter
    enum
    {
        PAGE_HEADER = 0,
        PAGE_BODY_LAST = 6,
        PAGE_SIGNATURE = 7,
        PAGE_NUM = 8,
    };

    TextAreaMail(); // 0x004FACFC
    ~TextAreaMail() override; // 0x004F764C slot 0x00 (falls into ~TextAreaEx)
    // 0x004FAD3C slot 0x04 (deleting dtor)
    void OnEvent(Widget* sender, s32 event) override; // 0x004FA898 slot 0x10
    u32 GetPageNum() override { return PAGE_NUM; } // 0x004FA890 slot 0x18

    // (inline)
    lyt::TextAreaMail* GetTextAreaMail() { return static_cast<lyt::TextAreaMail*>(m_pTextArea); }
};
ASSERT_SIZE(TextAreaMail, 0x34);
} // namespace gui
