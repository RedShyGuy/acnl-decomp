#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
// RTTI N3gui15CellPhoneKeySetE @ 0x008D0E6C
// vtable 0x00903F9C (vptr 0x00903FA4), offset_to_top 0, 9 entries
class CellPhoneKeySet : public ::gui::KeySet
{
public:
    CellPhoneKeySet(); // ctor candidate(s) 0x004FDAE0 (unverified)
    virtual ~CellPhoneKeySet(); // 0x004FDCA0 slot 0x00 | slot vf_0x00 of gui::KeySet
    // 0x004FDBCC slot 0x04 | slot vf_0x04 of gui::KeySet (deleting dtor)
    virtual void vf_0x08(); // 0x004FB9A8 slot 0x08 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x0C(); // 0x004FD2C8 slot 0x0C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x10(); // 0x004FD730 slot 0x10 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x14(); // 0x004FB8BC slot 0x14 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x18(); // 0x004FD720 slot 0x18 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x1C(); // 0x004FD50C slot 0x1C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x20(); // 0x004FD718 slot 0x20 | virtual slot, introduced by gui::KeySet
};
} // namespace gui
