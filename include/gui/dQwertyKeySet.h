#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
// RTTI N3gui12QwertyKeySetE @ 0x008D0E18
// vtable 0x00903E0C (vptr 0x00903E14), offset_to_top 0, 10 entries
class QwertyKeySet : public ::gui::KeySet
{
public:
    QwertyKeySet(); // ctor candidate(s) 0x004F9CD8 (unverified)
    virtual ~QwertyKeySet(); // 0x004F9E5C slot 0x00 | slot vf_0x00 of gui::KeySet
    // 0x004F9D9C slot 0x04 | slot vf_0x04 of gui::KeySet (deleting dtor)
    virtual void vf_0x08(); // 0x004F8238 slot 0x08 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x0C(); // 0x004F93D4 slot 0x0C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x10(); // 0x004F9700 slot 0x10 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x14(); // 0x004F8004 slot 0x14 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x18(); // 0x004F96F0 slot 0x18 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x1C(); // 0x004F9520 slot 0x1C | virtual slot, introduced by gui::KeySet
    virtual void vf_0x20(); // 0x004F96E8 slot 0x20 | virtual slot, introduced by gui::KeySet
    virtual void vf_0x24(); // 0x004F80D4 slot 0x24 | virtual slot, introduced by gui::QwertyKeySet
};
} // namespace gui
