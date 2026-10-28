#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadThreadMgr.h"

// RTTI N4sead9ThreadMgr18SingletonDisposer_E @ 0x008D23D8
// vtable 0x00907130 (vptr 0x00907138), offset_to_top 0, 2 entries
//
// Destroys the instance with the heap it is on (in the block of the instance, at +4).
class sead::ThreadMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x00562AB4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00562A5C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
