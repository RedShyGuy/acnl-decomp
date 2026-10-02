#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x140E0 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryPWorks : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryPWorks(); // ctor address unknown
    virtual ~AcNpcSpSecretaryPWorks(); // ModuleRealVillage.cro +0x00F88C slot 0x00
};
