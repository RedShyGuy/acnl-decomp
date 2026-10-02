#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "ugc/dMgr.h"

// RTTI N3ugc3Mgr18SingletonDisposer_E @ 0x008D1048
// vtable 0x009047E0 (vptr 0x009047E8), offset_to_top 0, 2 entries
class ugc::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00524D70 (unverified)
    virtual ~SingletonDisposer_(); // 0x00524E64 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00524DDC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
