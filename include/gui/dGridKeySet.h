#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
// RTTI N3gui10GridKeySetE @ 0x008D0DD0
// vtable 0x00903D00 (vptr 0x00903D08), offset_to_top 0, 9 entries
class GridKeySet : public ::gui::KeySet
{
public:
    GridKeySet(); // ctor candidate(s) 0x004F54E0 (unverified)
    virtual ~GridKeySet(); // 0x004F5650 slot 0x00 | slot vf_0x00 of gui::KeySet
    // 0x004F559C slot 0x04 | slot vf_0x04 of gui::KeySet (deleting dtor)
    virtual void vf_0x08(); // 0x004F4224 slot 0x08 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x0C(); // 0x004F4F10 slot 0x0C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x10(); // 0x004F5298 slot 0x10 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x14(); // 0x004F4100 slot 0x14 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x18(); // 0x004F5288 slot 0x18 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x1C(); // 0x004F5040 slot 0x1C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x20(); // 0x004F5280 slot 0x20 | virtual slot, introduced by gui::KeySet
};
} // namespace gui
