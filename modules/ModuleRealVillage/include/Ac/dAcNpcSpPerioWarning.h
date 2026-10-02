#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13858 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpPerioWarning : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPerioWarning(); // ctor address unknown
    virtual ~AcNpcSpPerioWarning(); // ModuleRealVillage.cro +0x008548 slot 0x00
};
