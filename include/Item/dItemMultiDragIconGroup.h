#pragma once

#include "decomp.h"
#include "Other/dDragIconGroup.h"

// RTTI 22ItemMultiDragIconGroup @ 0x008CCDEC
// vtable 0x008F6A54 (vptr 0x008F6A5C), offset_to_top 0, 2 entries
// vtable 0x008F6A64 (vptr 0x008F6A6C), offset_to_top -12, 2 entries
class ItemMultiDragIconGroup : public ::DragIconGroup
{
public:
    ItemMultiDragIconGroup(); // ctor candidate(s) 0x00330308 (unverified)
    virtual ~ItemMultiDragIconGroup(); // 0x00330380 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00330328 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
