#pragma once

#include "decomp.h"
#include "gui/dTextArea.h"

namespace gui {
// RTTI N3gui10TextAreaExE @ 0x008D0DE8
// vtable 0x00903D58 (vptr 0x00903D60), offset_to_top 0, 7 entries
class TextAreaEx : public ::gui::TextArea
{
public:
    TextAreaEx(); // ctor address unknown
    virtual ~TextAreaEx(); // 0x004F7650 slot 0x00 | slot vf_0x00 of gui::TextArea
    // 0x004F763C slot 0x04 | slot vf_0x04 of gui::TextArea (deleting dtor)
    virtual void vf_0x0C(); // 0x004F73C0 slot 0x0C | virtual slot, introduced by gui::TextArea
    virtual void vf_0x14(); // 0x004F6FC8 slot 0x14 | virtual slot, introduced by gui::TextAreaEx
    virtual void vf_0x18(); // 0x004F6F80 slot 0x18 | virtual slot, introduced by gui::TextAreaEx
};
} // namespace gui
