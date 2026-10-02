#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpJonny.h"
#include "Npc/dNpcSimpleModel.h"

// vtable +0x146FC in ModuleRealVillage.cro, offset_to_top 0, 20 entries
class AcNpcSpJonny::MyModel : public ::NpcSimpleModel
{
public:
    MyModel(); // ctor address unknown
    virtual ~MyModel(); // ModuleRealVillage.cro +0x002CAC slot 0x0C
};
