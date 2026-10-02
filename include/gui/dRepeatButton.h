#pragma once

#include "decomp.h"
#include "gui/dButtonBase.h"

namespace gui {
// RTTI N3gui12RepeatButtonE @ 0x008D0E24
// vtable 0x00903E3C (vptr 0x00903E44), offset_to_top 0, 15 entries
class RepeatButton : public ::gui::ButtonBase
{
public:
    RepeatButton(); // ctor candidate(s) 0x004FA784 (unverified)
    virtual void vf_0x00(); // 0x004FA864 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004FA834 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x08(); // 0x004FA69C slot 0x08 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x00747578 slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x28(); // 0x004F9F50 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004FA194 slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x34(); // 0x004FA59C slot 0x34 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x38(); // 0x004FA0EC slot 0x38 | virtual slot, introduced by gui::ButtonBase
};
} // namespace gui
