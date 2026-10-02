#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"
#include "sead/seadIDelegate1.h"

// vtable +0xDF40 in ModuleAutoCamp.cro, offset_to_top 0, 2 entries
class RhandPartsAnimSetter : public ::sead::IDelegate1<HumanModel*>
{
public:
    RhandPartsAnimSetter(); // ctor address unknown
};
