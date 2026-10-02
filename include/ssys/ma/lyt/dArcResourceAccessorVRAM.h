#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ArcResourceAccessor.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt23ArcResourceAccessorVRAME @ 0x008D2484
// vtable 0x009072EC (vptr 0x009072F4), offset_to_top 0, 7 entries
class ArcResourceAccessorVRAM : public ::nw::lyt::ArcResourceAccessor
{
public:
    ArcResourceAccessorVRAM(); // ctor address unknown
    virtual ~ArcResourceAccessorVRAM(); // 0x00569024 slot 0x00 | slot vf_0x00 of nw::lyt::ResourceAccessor
    // 0x00569014 slot 0x04 | slot vf_0x04 of nw::lyt::ResourceAccessor (deleting dtor)
    virtual void vf_0x14(); // 0x00568BF8 slot 0x14 | virtual slot, introduced by nw::lyt::ResourceAccessor
};
} // namespace lyt
} // namespace ma
} // namespace ssys
