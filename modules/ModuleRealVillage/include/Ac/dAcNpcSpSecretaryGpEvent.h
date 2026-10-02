#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x143B8 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryGpEvent : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryGpEvent(); // ctor address unknown
    virtual ~AcNpcSpSecretaryGpEvent(); // ModuleRealVillage.cro +0x010AF0 slot 0x00
};
