#pragma once

#include "decomp.h"
#include "netgame/dNetGameMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N7netgame10NetGameMgr18SingletonDisposer_E @ 0x008D3F2C
// vtable 0x0090BD04 (vptr 0x0090BD0C), offset_to_top 0, 2 entries
class netgame::NetGameMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, sead::IDisposer::HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x00618BF4 slot 0x00
    // 0x00618BA4 slot 0x04 (deleting dtor)
};
