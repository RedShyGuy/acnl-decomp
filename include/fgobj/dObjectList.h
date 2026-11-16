#pragma once

#include "decomp.h"
#include "ssys/st/dListPriority.h"

namespace fgobj {
// The objects of the Proc, sorted by their sort key (class name from the RTTI).
// RTTI N5fgobj10ObjectListE @ 0x008D294C
// vtable 0x00908AF4 (vptr 0x00908AFC), offset_to_top 0, 2 entries
class ObjectList : public ::ssys::st::ListPriority
{
public:
    ObjectList() {}
    virtual ~ObjectList(); // 0x00592564 slot 0x00 | slot vf_0x00 of ssys::st::List
};
} // namespace fgobj
