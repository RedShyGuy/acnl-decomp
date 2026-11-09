#pragma once

#include "decomp.h"
#include "script/dProject.h"
#include "sead/seadIDisposer.h"

// RTTI N6script7Project18SingletonDisposer_E @ 0x008D3AE4
// vtable 0x0090A950 (vptr 0x0090A958), offset_to_top 0, 2 entries
// Destroys the project when its heap is destroyed (in the first 16 bytes of the project).
class script::Project::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x005F7DC0 slot 0x00 (name after sead)
    // 0x005F7D5C slot 0x04 (deleting dtor)
};
