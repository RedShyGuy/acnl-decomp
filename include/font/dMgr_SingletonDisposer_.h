#pragma once

#include "decomp.h"
#include "font/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N4font3Mgr18SingletonDisposer_E @ 0x008D10E4
// vtable 0x00904864 (vptr 0x0090486C), offset_to_top 0, 2 entries
//
// Destroys the font manager when its heap is destroyed (in the first 16 bytes of the manager).
class font::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x0052ECFC slot 0x00 (name after sead)
    // 0x0052ECAC slot 0x04 (deleting dtor)
};
