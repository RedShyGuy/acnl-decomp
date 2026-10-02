#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt8BoundingE @ 0x008D08B4
// vtable 0x00902E20 (vptr 0x00902E28), offset_to_top 0, 28 entries
class Bounding : public ::nw::lyt::Pane
{
public:
    Bounding(); // ctor candidate(s) 0x004BC8D0 (unverified)
    virtual ~Bounding(); // 0x004B7FC0 slot 0x00 | slot vf_0x00 of nw::lyt::Pane
    // 0x004BC8E8 slot 0x04 | slot vf_0x04 of nw::lyt::Pane (deleting dtor)
    virtual void GetRuntimeTypeInfo() const; // 0x0073E84C slot 0x08 | slot vf_0x08 of nw::lyt::Pane
    virtual void vf_0x64(); // 0x004BC8CC slot 0x64 | virtual slot, introduced by nw::lyt::Pane
    Bounding(const nw::lyt::res::Bounding*, const nw::lyt::ResBlockSet&); // 0x004BC8D0 | nintendogs:callseq-callee [tier A]
};
} // namespace lyt
} // namespace nw
