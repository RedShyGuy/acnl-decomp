#pragma once

#include "decomp.h"
#include "gui/dButtonEx.h"

namespace gui {
// RTTI N3gui13RadioButtonExE @ 0x008D0E3C
// vtable 0x00903EA4 (vptr 0x00903EAC), offset_to_top 0, 15 entries
class RadioButtonEx : public ::gui::ButtonEx
{
public:
    RadioButtonEx(); // ctor address unknown
    virtual void vf_0x00(); // 0x004FAFD4 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004FAFA4 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x00747584 slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x2C(); // 0x004FAE84 slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x30(); // 0x004FAE54 slot 0x30 | virtual slot, introduced by gui::ButtonBase
};
} // namespace gui
