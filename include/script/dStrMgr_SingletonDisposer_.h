#pragma once

#include "decomp.h"
#include "script/dStrMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script6StrMgr18SingletonDisposer_E @ 0x008D3AAC
// vtable 0x0090A8DC (vptr 0x0090A8E4), offset_to_top 0, 2 entries
class script::StrMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x005F3DC8 (unverified)
    virtual ~SingletonDisposer_(); // 0x005F3ECC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005F3E78 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
