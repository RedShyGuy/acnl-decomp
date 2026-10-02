#pragma once

#include "decomp.h"
#include "gui/dKey.h"

namespace gui {
// RTTI N3gui6MulKeyE @ 0x008D0EA0
// vtable 0x0090409C (vptr 0x009040A4), offset_to_top 0, 6 entries
class MulKey : public ::gui::Key
{
public:
    MulKey(); // ctor address unknown
    virtual ~MulKey(); // 0x004FE87C slot 0x00 | slot vf_0x00 of gui::Key
    // 0x004FE830 slot 0x04 | slot vf_0x04 of gui::Key (deleting dtor)
    virtual void vf_0x08(); // 0x004FE82C slot 0x08 | virtual slot, introduced by gui::Key
    virtual void vf_0x0C(); // 0x0074783C slot 0x0C | virtual slot, introduced by gui::Key
    virtual void vf_0x10(); // 0x004FE824 slot 0x10 | virtual slot, introduced by gui::Key
    virtual void vf_0x14(); // 0x004FE7D8 slot 0x14 | virtual slot, introduced by gui::Key
};
} // namespace gui
