#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"
#include "sead/seadIDelegate1.h"

// vtable +0xDF50 in ModuleAutoCamp.cro, offset_to_top 0, 2 entries
class CoffeePartsAnimSetter : public ::sead::IDelegate1<HumanModel*>
{
public:
    CoffeePartsAnimSetter(); // ctor address unknown
};
