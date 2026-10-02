#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadResourceMgr.h"

// RTTI N4sead11ResourceMgr18SingletonDisposer_E @ 0x008D1424
// vtable 0x00905040 (vptr 0x00905048), offset_to_top 0, 2 entries
class sead::ResourceMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0012C43C (unverified)
    virtual ~SingletonDisposer_(); // 0x00540B30 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00540AC8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
