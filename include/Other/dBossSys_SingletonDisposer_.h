#pragma once

#include "decomp.h"
#include "Other/dBossSys.h"
#include "sead/seadIDisposer.h"

// RTTI N7BossSys18SingletonDisposer_E @ 0x008D3EC4
// vtable 0x0090BC48 (vptr 0x0090BC50), offset_to_top 0, 2 entries
class BossSys::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x00606D08 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00606C7C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
