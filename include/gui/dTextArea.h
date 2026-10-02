#pragma once

#include "decomp.h"

namespace gui {
// RTTI N3gui8TextAreaE @ 0x008D0EE4
// vtable 0x009041A4 (vptr 0x009041AC), offset_to_top 0, 5 entries
class TextArea
{
public:
    TextArea(); // ctor candidate(s) 0x00500758 (unverified)
    virtual ~TextArea(); // 0x005007F8 slot 0x00 | slot vf_0x00 of gui::TextArea
    // 0x00500788 slot 0x04 | slot vf_0x04 of gui::TextArea (deleting dtor)
    virtual void vf_0x08(); // 0x005001C4 slot 0x08 | virtual slot, introduced by gui::TextArea
    virtual void vf_0x0C(); // 0x00500420 slot 0x0C | virtual slot, introduced by gui::TextArea
    virtual void vf_0x10(); // 0x00500590 slot 0x10 | virtual slot, introduced by gui::TextArea
};
} // namespace gui
