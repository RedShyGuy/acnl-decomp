#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x130E4 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpGraceOut : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpGraceOut(); // ctor address unknown
    virtual ~AcNpcSpGraceOut(); // ModuleRealVillage.cro +0x005758 slot 0x00
};
