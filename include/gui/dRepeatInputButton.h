#pragma once

#include "decomp.h"
#include "gui/dRepeatButtonEx.h"

namespace gui {
// RTTI N3gui17RepeatInputButtonE @ 0x008D0E84
// vtable 0x0090400C (vptr 0x00904014), offset_to_top 0, 15 entries
class RepeatInputButton : public ::gui::RepeatButtonEx
{
public:
    RepeatInputButton(); // ctor candidate(s) 0x004FE3B8 (unverified)
    virtual void vf_0x00(); // 0x004FE518 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x004FE4E8 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x00747830 slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x28(); // 0x004FE178 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004FE214 slot 0x2C | virtual slot, introduced by gui::ButtonBase
};
} // namespace gui
