#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "ssys/ma/dFlushCache.h"

// RTTI N4ssys2ma10FlushCache18SingletonDisposer_E @ 0x008D23F8
// vtable 0x0090717C (vptr 0x00907184), offset_to_top 0, 2 entries
class ssys::ma::FlushCache::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00567138 (unverified)
    virtual ~SingletonDisposer_(); // 0x00567218 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005671C4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
