#pragma once

#include "decomp.h"
#include "script/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script3Mgr18SingletonDisposer_E @ 0x008D3A4C
// vtable 0x0090A514 (vptr 0x0090A51C), offset_to_top 0, 2 entries
class script::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x005EA318 (unverified)
    virtual ~SingletonDisposer_(); // 0x005EA410 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005EA3CC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
