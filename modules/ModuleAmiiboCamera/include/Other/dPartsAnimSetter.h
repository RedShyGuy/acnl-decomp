#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"
#include "sead/seadIDelegate1.h"

// vtable +0x153B4 in ModuleAmiiboCamera.cro, offset_to_top 0, 2 entries
class PartsAnimSetter : public ::sead::IDelegate1<HumanModel*>
{
public:
    PartsAnimSetter(); // ctor address unknown
};
