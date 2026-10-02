#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x1E564 in ModuleIncSave.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryTakumiImport : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryTakumiImport(); // ctor address unknown
    virtual ~AcNpcSpSecretaryTakumiImport(); // ModuleIncSave.cro +0x00963C slot 0x00
    virtual void Calc(); // ModuleIncSave.cro +0x009318 slot 0x24
};
