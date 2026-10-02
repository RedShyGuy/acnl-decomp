#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"

namespace gui {
// RTTI N3gui8EditAreaE @ 0x008D0ED8
// vtable 0x00904158 (vptr 0x00904160), offset_to_top 0, 17 entries
class EditArea : public ::gui::TchOnButton
{
public:
    EditArea(); // ctor candidate(s) 0x00500054 (unverified)
    virtual void vf_0x00(); // 0x00500198 slot 0x00 | virtual slot, introduced by gui::Widget
    virtual void vf_0x04(); // 0x00500168 slot 0x04 | virtual slot, introduced by gui::Widget
    virtual void vf_0x0C(); // 0x0074756C slot 0x0C | virtual slot, introduced by gui::Widget
    virtual void vf_0x10(); // 0x004FF650 slot 0x10 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x28(); // 0x004FFB18 slot 0x28 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x2C(); // 0x004FFCD4 slot 0x2C | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x30(); // 0x004F7F5C slot 0x30 | virtual slot, introduced by gui::ButtonBase
    virtual void vf_0x3C(); // 0x004F7F1C slot 0x3C | virtual slot, introduced by gui::EditArea
    virtual void vf_0x40(); // 0x00747560 slot 0x40 | virtual slot, introduced by gui::EditArea
};
} // namespace gui
