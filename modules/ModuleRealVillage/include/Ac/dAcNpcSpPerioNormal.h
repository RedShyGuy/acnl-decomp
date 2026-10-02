#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13528 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpPerioNormal : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPerioNormal(); // ctor address unknown
    virtual ~AcNpcSpPerioNormal(); // ModuleRealVillage.cro +0x007970 slot 0x00
};
