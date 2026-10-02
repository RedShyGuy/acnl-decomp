#pragma once

#include "decomp.h"
#include "Other/dChoiceStandardWin.h"
#include "script/dChoiceStandardWin.h"

// vtable +0x21080 in ModuleVillage.cro, offset_to_top 0, 15 entries
class ChoiceStandardWin::LocalChoice : public ::script::ChoiceStandardWin
{
public:
    LocalChoice(); // ctor address unknown
    virtual ~LocalChoice(); // ModuleVillage.cro +0x00118C slot 0x00
};
