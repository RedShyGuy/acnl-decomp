#pragma once

#include "decomp.h"
#include "gui/dEditArea.h"

namespace gui {
// RTTI N3gui10EditAreaExE @ 0x008D0DC4
// vtable 0x00903CB4 (vptr 0x00903CBC), offset_to_top 0, 17 entries
class EditAreaEx : public ::gui::EditArea
{
public:
    EditAreaEx(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F40C0 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004F4090 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x28(); // 0x004F3B24 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004F3CD0 slot 0x2C | virtual slot, introduced by gui::ButtonBase
};
} // namespace gui
