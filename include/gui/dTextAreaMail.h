#pragma once

#include "decomp.h"
#include "gui/dTextAreaEx.h"

namespace gui {
// RTTI N3gui12TextAreaMailE @ 0x008D0E30
// vtable 0x00903E80 (vptr 0x00903E88), offset_to_top 0, 7 entries
class TextAreaMail : public ::gui::TextAreaEx
{
public:
    TextAreaMail(); // ctor candidate(s) 0x004FACFC (unverified)
    virtual ~TextAreaMail(); // 0x004F764C slot 0x00 | slot vf_0x00 of gui::TextArea
    // 0x004FAD3C slot 0x04 | slot vf_0x04 of gui::TextArea (deleting dtor)
    virtual void vf_0x10(); // 0x004FA898 slot 0x10 | virtual slot, introduced by gui::TextArea
    virtual void vf_0x18(); // 0x004FA890 slot 0x18 | virtual slot, introduced by gui::TextAreaEx
};
} // namespace gui
