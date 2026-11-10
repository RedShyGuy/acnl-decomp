#pragma once

#include "decomp.h"
#include "gui/dEditArea.h"
#include "lyt/dTextAreaEx.h"

namespace gui {
// The text field of a lyt::TextAreaEx (pages): scrolls faster, tells the text area about the scroll and the cursor,
// and sends its own events (9 pressed, 11 released). Only the class name is from the binary (RTTI); member and function
// names are ours.
// RTTI N3gui10EditAreaExE @ 0x008D0DC4
// vtable 0x00903CB4 (vptr 0x00903CBC), offset_to_top 0, 17 entries
class EditAreaEx : public ::gui::EditArea
{
public:
    // the events of the field
    enum
    {
        EVENT_PRESS = 9,
        EVENT_RELEASE = 11,
    };

    // (inline)
    EditAreaEx(const Description& desc, lyt::TextAreaEx* textArea, lyt::Object* keyLayout)
        : EditArea(desc, textArea), m_pKeyLayout(keyLayout)
    {
    }
    ~EditAreaEx() override; // 0x004F40C0 slot 0x00
    // 0x004F4090 slot 0x04 (deleting dtor)
    void UpdateIdle() override; // 0x004F3B24 slot 0x28
    void UpdatePressed() override; // 0x004F3CD0 slot 0x2C

    // (inline) the text area as the lyt::TextAreaEx it is
    lyt::TextAreaEx* GetTextAreaEx() { return static_cast<lyt::TextAreaEx*>(m_pTextArea); }

    lyt::Object* m_pKeyLayout;  // 0x148 the layout of the keys of gui::TextAreaEx (never read)
};
ASSERT_SIZE(EditAreaEx, 0x14C);
} // namespace gui
