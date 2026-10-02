#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x12DB4 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpJonny : public ::AcNpcSp
{
public:
    class MyModel;
    class TalkRecept;
    AcNpcSpJonny(); // ctor address unknown
    virtual ~AcNpcSpJonny(); // ModuleRealVillage.cro +0x0030A8 slot 0x00
    virtual void Calc(); // ModuleRealVillage.cro +0x002EB4 slot 0x24
};
