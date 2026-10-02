#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt10LayoutListE @ 0x008D2440
// vtable 0x0090725C (vptr 0x00907264), offset_to_top 0, 3 entries
class LayoutList : public ::ssys::st::List
{
public:
    LayoutList(); // ctor address unknown
    virtual ~LayoutList(); // 0x0056859C slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00568598 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void vf_0x08(); // 0x00568568 slot 0x08 | virtual slot, introduced by ssys::ma::lyt::LayoutList
};
} // namespace lyt
} // namespace ma
} // namespace ssys
