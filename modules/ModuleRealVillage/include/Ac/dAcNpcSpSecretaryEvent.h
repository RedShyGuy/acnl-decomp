#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13E08 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryEvent : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryEvent(); // ctor address unknown
    virtual ~AcNpcSpSecretaryEvent(); // ModuleRealVillage.cro +0x00C690 slot 0x00
    virtual void Calc(); // ModuleRealVillage.cro +0x00C480 slot 0x24
};
