#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadThreadMgr.h"

// RTTI N4sead9ThreadMgr18SingletonDisposer_E @ 0x008D23D8
// vtable 0x00907130 (vptr 0x00907138), offset_to_top 0, 2 entries
class sead::ThreadMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00133888 (unverified)
    virtual ~SingletonDisposer_(); // 0x00562AB4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00562A5C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
