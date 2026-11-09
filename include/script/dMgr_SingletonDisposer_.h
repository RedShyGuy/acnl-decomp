#pragma once

#include "decomp.h"
#include "script/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script3Mgr18SingletonDisposer_E @ 0x008D3A4C
// vtable 0x0090A514 (vptr 0x0090A51C), offset_to_top 0, 2 entries
//
// Destroys the script manager when its heap is destroyed (in the first 16 bytes of the manager).
class script::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x005EA410 slot 0x00 (name after sead)
    // 0x005EA3CC slot 0x04 (deleting dtor)
};
