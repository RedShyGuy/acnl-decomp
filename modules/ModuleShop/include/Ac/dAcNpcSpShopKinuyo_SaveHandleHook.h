#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopKinuyo.h"
#include "sead/seadIDelegateR.h"

// vtable +0x28D20 in ModuleShop.cro, offset_to_top 0, 2 entries
class AcNpcSpShopKinuyo::SaveHandleHook : public ::sead::IDelegateR<bool>
{
public:
    SaveHandleHook(); // ctor address unknown
};
