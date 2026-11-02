#pragma once

#include "decomp.h"
#include "ssys/st/dListPriority.h"

namespace ssys {
namespace ma {
// the free or the used blocks of a VramExpHeap
// RTTI N4ssys2ma11VramMemListE @ 0x008D2410
// vtable 0x009071C0 (vptr 0x009071C8), offset_to_top 0, 2 entries
class VramMemList : public ::ssys::st::ListPriority
{
public:
    VramMemList() {} // (inline in the constructor of VramExpHeap)
    virtual ~VramMemList(); // 0x00567AA0 slot 0x00
};
ASSERT_SIZE(VramMemList, 0xC);
} // namespace ma
} // namespace ssys
