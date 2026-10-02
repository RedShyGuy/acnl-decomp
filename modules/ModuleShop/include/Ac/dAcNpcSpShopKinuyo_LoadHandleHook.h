#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopKinuyo.h"
#include "sead/seadIDelegateR.h"

// vtable +0x28D10 in ModuleShop.cro, offset_to_top 0, 2 entries
class AcNpcSpShopKinuyo::LoadHandleHook : public ::sead::IDelegateR<bool>
{
public:
    LoadHandleHook(); // ctor address unknown
};
