#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x8A14 in ModulePrologue.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryPlantTree : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryPlantTree(); // ctor address unknown
    virtual ~AcNpcSpSecretaryPlantTree(); // ModulePrologue.cro +0x00694C slot 0x00
    virtual void Draw(); // ModulePrologue.cro +0x000571 slot 0x30
};
