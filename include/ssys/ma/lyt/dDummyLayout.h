#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Layout.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt11DummyLayoutE @ 0x008D2458
// vtable 0x00907280 (vptr 0x00907288), offset_to_top 0, 17 entries
class DummyLayout : public ::nw::lyt::Layout
{
public:
    DummyLayout(); // ctor candidate(s) 0x0056A0CC (unverified)
    virtual ~DummyLayout(); // 0x004BAA64 slot 0x00 | slot vf_0x00 of nw::lyt::Layout
    // 0x00568798 slot 0x04 | slot vf_0x04 of nw::lyt::Layout (deleting dtor)
    virtual void CreateAnimTransform(const void*, nw::lyt::ResourceAccessor*); // 0x00568770 slot 0x10 | slot vf_0x10 of nw::lyt::Layout
    virtual void vf_0x28(); // 0x00568674 slot 0x28 | virtual slot, introduced by nw::lyt::Layout
};
} // namespace lyt
} // namespace ma
} // namespace ssys
