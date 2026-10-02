#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"

// RTTI 6ListId @ 0x008CD2FC
// vtable 0x008F8C94 (vptr 0x008F8C9C), offset_to_top 0, 2 entries
class ListId : public ::ssys::st::List
{
public:
    ListId(); // ctor candidate(s) 0x007893C0 (unverified)
    virtual ~ListId(); // 0x005C025C slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x005C0258 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
