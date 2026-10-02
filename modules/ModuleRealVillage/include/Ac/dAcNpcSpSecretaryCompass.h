#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x1424C in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryCompass : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryCompass(); // ctor address unknown
    virtual ~AcNpcSpSecretaryCompass(); // ModuleRealVillage.cro +0x00FF58 slot 0x00
};
