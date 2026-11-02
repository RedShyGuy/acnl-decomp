#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "ssys/ma/dFlushCache.h"

// destroys the FlushCache when its heap is destroyed (in the first 16 bytes of the FlushCache)
// RTTI N4ssys2ma10FlushCache18SingletonDisposer_E @ 0x008D23F8
// vtable 0x0090717C (vptr 0x00907184), offset_to_top 0, 2 entries
class ssys::ma::FlushCache::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x00567218 slot 0x00
};
