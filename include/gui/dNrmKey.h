#pragma once

#include "decomp.h"
#include "gui/dKey.h"

namespace gui {
// RTTI N3gui6NrmKeyE @ 0x008D0EAC
// vtable 0x009040BC (vptr 0x009040C4), offset_to_top 0, 6 entries
class NrmKey : public ::gui::Key
{
public:
    NrmKey(); // ctor candidate(s) 0x004FE914 (unverified)
    virtual ~NrmKey(); // 0x004FEA28 slot 0x00 | slot vf_0x00 of gui::Key
    // 0x004FE9E4 slot 0x04 | slot vf_0x04 of gui::Key (deleting dtor)
    virtual void vf_0x08(); // 0x004FE910 slot 0x08 | virtual slot, introduced by gui::Key
    virtual void vf_0x0C(); // 0x00747848 slot 0x0C | virtual slot, introduced by gui::Key
    virtual void vf_0x10(); // 0x004FE908 slot 0x10 | virtual slot, introduced by gui::Key
    virtual void vf_0x14(); // 0x004FE8BC slot 0x14 | virtual slot, introduced by gui::Key
};
} // namespace gui
