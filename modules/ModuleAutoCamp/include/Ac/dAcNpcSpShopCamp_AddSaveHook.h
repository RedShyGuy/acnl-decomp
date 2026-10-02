#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopCamp.h"
#include "sead/seadIDelegateR.h"

// vtable +0xE1C8 in ModuleAutoCamp.cro, offset_to_top 0, 2 entries
class AcNpcSpShopCamp::AddSaveHook : public ::sead::IDelegateR<bool>
{
public:
    AddSaveHook(); // ctor address unknown
};
