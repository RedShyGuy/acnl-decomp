#pragma once

#include "decomp.h"
#include "gui/dButtonBase.h"

namespace gui {
// RTTI N3gui11RadioButtonE @ 0x008D0E00
// vtable 0x00903DC8 (vptr 0x00903DD0), offset_to_top 0, 15 entries
class RadioButton : public ::gui::ButtonBase
{
public:
    RadioButton(); // ctor candidate(s) 0x004F7E14 (unverified)
    virtual void vf_0x00(); // 0x004F7EF0 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004F7EC0 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x08(); // 0x004F7D80 slot 0x08 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x00747554 slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x28(); // 0x004F7A1C slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004F7B90 slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x30(); // 0x004F7AC4 slot 0x30 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x34(); // 0x004F7CB4 slot 0x34 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x38(); // 0x004F7B38 slot 0x38 | virtual slot, introduced by gui::ButtonBase
};
} // namespace gui
