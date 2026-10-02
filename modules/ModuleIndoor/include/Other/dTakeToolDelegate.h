#pragma once

#include "decomp.h"
#include "sead/seadIDelegate2R.h"

// vtable +0x66700 in ModuleIndoor.cro, offset_to_top 0, 2 entries
class TakeToolDelegate : public ::sead::IDelegate2R<ssys::ma::Vec3 const&, ssys::ma::Vec3 const&, bool>
{
public:
    TakeToolDelegate(); // ctor address unknown
};
