#pragma once

#include "decomp.h"
#include "Other/dHomeBtnProhibition.h"
#include "sead/seadIDisposer.h"

// RTTI N18HomeBtnProhibition18SingletonDisposer_E @ 0x008CDB38
// vtable 0x008FBAD8 (vptr 0x008FBAE0), offset_to_top 0, 2 entries
class HomeBtnProhibition::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x002E32A0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x002E3248 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
