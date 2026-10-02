#pragma once

#include "decomp.h"
#include "sead/seadIDelegate2R.h"

// vtable +0x6699C in ModuleIndoor.cro, offset_to_top 0, 2 entries
class TakeOtherDelegate : public ::sead::IDelegate2R<ssys::ma::Vec3 const&, ssys::ma::Vec3 const&, bool>
{
public:
    TakeOtherDelegate(); // ctor address unknown
};
