#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
// RTTI N3gui10KanaKeySetE @ 0x008D0DDC
// vtable 0x00903D2C (vptr 0x00903D34), offset_to_top 0, 9 entries
class KanaKeySet : public ::gui::KeySet
{
public:
    KanaKeySet(); // ctor candidate(s) 0x004F6CFC (unverified)
    virtual ~KanaKeySet(); // 0x004F6E68 slot 0x00 | slot vf_0x00 of gui::KeySet
    // 0x004F6DA8 slot 0x04 | slot vf_0x04 of gui::KeySet (deleting dtor)
    virtual void vf_0x08(); // 0x004F5B08 slot 0x08 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x0C(); // 0x004F67DC slot 0x0C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x10(); // 0x004F6A3C slot 0x10 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x14(); // 0x004F56FC slot 0x14 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x18(); // 0x004F6A2C slot 0x18 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x1C(); // 0x004F6914 slot 0x1C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x20(); // 0x004F6A24 slot 0x20 | virtual slot, introduced by gui::KeySet
};
} // namespace gui
