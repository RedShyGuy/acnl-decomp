#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"

namespace fgobj {
// The objects to draw (class name from the RTTI).
// RTTI N5fgobj8DrawListE @ 0x008D2A30
// vtable 0x00908E2C (vptr 0x00908E34), offset_to_top 0, 2 entries
class DrawList : public ::ssys::st::List
{
public:
    DrawList() {}
    virtual ~DrawList(); // 0x005A3108 slot 0x00 | slot vf_0x00 of ssys::st::List
};
} // namespace fgobj
