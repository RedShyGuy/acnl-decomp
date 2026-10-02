#pragma once

#include "decomp.h"
#include "gui/dKey.h"

namespace gui {
// RTTI N3gui6TglKeyE @ 0x008D0EB8
// vtable 0x009040DC (vptr 0x009040E4), offset_to_top 0, 6 entries
class TglKey : public ::gui::Key
{
public:
    TglKey(); // ctor address unknown
    virtual ~TglKey(); // 0x004FE658 slot 0x00 | slot vf_0x00 of gui::Key
    // 0x004FEAB8 slot 0x04 | slot vf_0x04 of gui::Key (deleting dtor)
    virtual void vf_0x08(); // 0x004FEAB4 slot 0x08 | virtual slot, introduced by gui::Key
    virtual void vf_0x0C(); // 0x00747854 slot 0x0C | virtual slot, introduced by gui::Key
    virtual void vf_0x10(); // 0x004FEAAC slot 0x10 | virtual slot, introduced by gui::Key
    virtual void vf_0x14(); // 0x004FEA60 slot 0x14 | virtual slot, introduced by gui::Key
};
} // namespace gui
