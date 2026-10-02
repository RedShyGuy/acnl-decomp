#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"

namespace gui {
// RTTI N3gui11InputButtonE @ 0x008D0DF4
// vtable 0x00903D7C (vptr 0x00903D84), offset_to_top 0, 17 entries
class InputButton : public ::gui::TchOnButton
{
public:
    InputButton(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F7854 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004F7824 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x00747548 slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x10(); // 0x004FF650 slot 0x10 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x28(); // 0x004F7734 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004F77C0 slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x30(); // 0x004F7F5C slot 0x30 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x3C(); // 0x004F7F1C slot 0x3C | virtual slot, introduced by gui::EditArea
    virtual void vf_0x40(); // 0x00747560 slot 0x40 | virtual slot, introduced by gui::EditArea
};
} // namespace gui
