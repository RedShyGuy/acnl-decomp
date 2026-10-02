#pragma once

#include "decomp.h"
#include "gui/dTextAreaEx.h"

namespace gui {
// RTTI N3gui13TextAreaBoardE @ 0x008D0E54
// vtable 0x00903F34 (vptr 0x00903F3C), offset_to_top 0, 7 entries
class TextAreaBoard : public ::gui::TextAreaEx
{
public:
    TextAreaBoard(); // ctor candidate(s) 0x004FB514 (unverified)
    virtual ~TextAreaBoard(); // 0x004FB564 slot 0x00 | slot vf_0x00 of gui::TextArea
    // 0x004FB554 slot 0x04 | slot vf_0x04 of gui::TextArea (deleting dtor)
    virtual void vf_0x10(); // 0x004FB248 slot 0x10 | virtual slot, introduced by gui::TextArea
    virtual void vf_0x18(); // 0x004FB240 slot 0x18 | virtual slot, introduced by gui::TextAreaEx
};
} // namespace gui
