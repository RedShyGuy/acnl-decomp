#pragma once

#include "decomp.h"
#include "ssys/st/dListPriority.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma11VramMemListE @ 0x008D2410
// vtable 0x009071C0 (vptr 0x009071C8), offset_to_top 0, 2 entries
class VramMemList : public ::ssys::st::ListPriority
{
public:
    VramMemList(); // ctor address unknown
    virtual ~VramMemList(); // 0x00567AA0 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00567A9C slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
} // namespace ma
} // namespace ssys
