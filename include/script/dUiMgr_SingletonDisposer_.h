#pragma once

#include "decomp.h"
#include "script/dUiMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script5UiMgr18SingletonDisposer_E @ 0x008D3A78
// vtable 0x0090A584 (vptr 0x0090A58C), offset_to_top 0, 2 entries
//
// Destroys the ui manager when its heap is destroyed (in the first 16 bytes of the manager).
class script::UiMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x005EB1D4 slot 0x00 (name after sead)
    // 0x005EB134 slot 0x04 (deleting dtor)
};
