#pragma once

#include "decomp.h"
#include "Other/dBossMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N7BossMgr18SingletonDisposer_E @ 0x008D3EB8
// vtable 0x0090BC38 (vptr 0x0090BC40), offset_to_top 0, 2 entries
class BossMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00606108 (unverified)
    virtual ~SingletonDisposer_(); // 0x006062D8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00606294 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
