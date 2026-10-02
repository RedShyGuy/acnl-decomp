#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13C9C in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpPerioTutorial : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPerioTutorial(); // ctor address unknown
    virtual ~AcNpcSpPerioTutorial(); // ModuleRealVillage.cro +0x00B894 slot 0x00
};
