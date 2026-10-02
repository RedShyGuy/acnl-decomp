#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x873C in ModulePrologue.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryOut : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryOut(); // ctor address unknown
    virtual ~AcNpcSpSecretaryOut(); // ModulePrologue.cro +0x003358 slot 0x00
    virtual void Calc(); // ModulePrologue.cro +0x002FD8 slot 0x24
};
