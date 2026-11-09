#pragma once

#include "decomp.h"
#include "script/dStrMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script6StrMgr18SingletonDisposer_E @ 0x008D3AAC
// vtable 0x0090A8DC (vptr 0x0090A8E4), offset_to_top 0, 2 entries
//
// Destroys the string manager when its heap is destroyed (in the first 16 bytes of the manager).
class script::StrMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x005F3ECC slot 0x00 (name after sead)
    // 0x005F3E78 slot 0x04 (deleting dtor)
};
