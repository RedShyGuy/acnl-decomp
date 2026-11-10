#pragma once

#include "decomp.h"
#include "lyt/dObject.h"

namespace lyt {
// an Object of the game with a vtable of its own (nothing is overridden; only the class name is from the RTTI)
// RTTI N3lyt8ObjectExE @ 0x008D0F90
// vtable 0x0090455C (vptr 0x00904564), offset_to_top 0, 4 entries
class ObjectEx : public ::lyt::Object
{
public:
    // (inline)
    ObjectEx(u32 unknown, s32 priority, sead::Heap* heap) : Object(unknown, priority, heap) {}
    virtual ~ObjectEx(); // 0x0050A3AC slot 0x00 (a nop that falls into the destructor of Object)
    // 0x0050A7EC slot 0x04 (deleting dtor)
};
} // namespace lyt
