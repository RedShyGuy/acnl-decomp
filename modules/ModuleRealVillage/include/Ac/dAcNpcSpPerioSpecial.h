#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x136EC in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpPerioSpecial : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPerioSpecial(); // ctor address unknown
    virtual ~AcNpcSpPerioSpecial(); // ModuleRealVillage.cro +0x007FD0 slot 0x00
};
