#pragma once

#include "decomp.h"
#include "Other/dFriendList.h"
#include "sead/seadIDisposer.h"

// RTTI N10FriendList18SingletonDisposer_E @ 0x008CD820
// vtable 0x008FA964 (vptr 0x008FA96C), offset_to_top 0, 2 entries
class FriendList::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0011DCDC (unverified)
    virtual ~SingletonDisposer_(); // 0x001AACB8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001AAC4C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
