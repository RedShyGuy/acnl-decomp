#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"

// vtable +0x8D364 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x8D908 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x96134 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x96244 in ModuleOutdoor.cro, offset_to_top -480, 11 entries
// vtable +0x8DD5C in ModuleOutdoor.cro, offset_to_top -576, 11 entries
// vtable +0x8D7B8 in ModuleOutdoor.cro, offset_to_top -588, 11 entries
class AcInsectFieldSwim : public ::AcInsectFieldBase
{
public:
    AcInsectFieldSwim(); // ctor address unknown
    virtual ~AcInsectFieldSwim(); // ModuleOutdoor.cro +0x04E230 slot 0x00
};
