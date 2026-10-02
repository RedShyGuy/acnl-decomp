#pragma once

#include "decomp.h"
#include "netgame/dNetGameMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N7netgame10NetGameMgr18SingletonDisposer_E @ 0x008D3F2C
// vtable 0x0090BD04 (vptr 0x0090BD0C), offset_to_top 0, 2 entries
class netgame::NetGameMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00618294 (unverified)
    virtual ~SingletonDisposer_(); // 0x00618BF4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00618BA4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
