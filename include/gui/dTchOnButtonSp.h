#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"

namespace gui {
// RTTI N3gui13TchOnButtonSpE @ 0x008D0E48
// vtable 0x00903EE8 (vptr 0x00903EF0), offset_to_top 0, 17 entries
class TchOnButtonSp : public ::gui::TchOnButton
{
public:
    TchOnButtonSp(); // ctor candidate(s) 0x004FB0E4 (unverified)
    virtual void vf_0x00(); // 0x004FB214 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004FB1E4 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x0074756C slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x10(); // 0x004FF650 slot 0x10 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x28(); // 0x004FB000 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004F7FAC slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x30(); // 0x004F7F5C slot 0x30 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x3C(); // 0x004F7F1C slot 0x3C | virtual slot, introduced by gui::EditArea
    virtual void vf_0x40(); // 0x00747560 slot 0x40 | virtual slot, introduced by gui::EditArea
};
} // namespace gui
