#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x1E3F8 in ModuleIncSave.cro, offset_to_top 0, 89 entries
class AcNpcSpSeniorTakumiImport : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSeniorTakumiImport(); // ctor address unknown
    virtual ~AcNpcSpSeniorTakumiImport(); // ModuleIncSave.cro +0x008D5C slot 0x00
    virtual void Calc(); // ModuleIncSave.cro +0x008AA0 slot 0x24
};
