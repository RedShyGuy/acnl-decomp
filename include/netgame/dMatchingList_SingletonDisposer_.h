#pragma once

#include "decomp.h"
#include "netgame/dMatchingList.h"
#include "sead/seadIDisposer.h"

// RTTI N7netgame12MatchingList18SingletonDisposer_E @ 0x008D3F38
// vtable 0x0090BD14 (vptr 0x0090BD1C), offset_to_top 0, 2 entries
class netgame::MatchingList::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0062044C (unverified)
    virtual ~SingletonDisposer_(); // 0x006206B4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0062064C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
