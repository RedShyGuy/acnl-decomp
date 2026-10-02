#pragma once

#include "decomp.h"
#include "Other/dCaution.h"
#include "sead/seadIDisposer.h"

// RTTI N7Caution18SingletonDisposer_E @ 0x008D3EDC
// vtable 0x0090BC74 (vptr 0x0090BC7C), offset_to_top 0, 2 entries
class Caution::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x0060CDE8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0060CD90 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
