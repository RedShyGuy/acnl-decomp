#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13B30 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpCleaningVisit : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpCleaningVisit(); // ctor address unknown
    virtual ~AcNpcSpCleaningVisit(); // ModuleRealVillage.cro +0x00B450 slot 0x00
};
