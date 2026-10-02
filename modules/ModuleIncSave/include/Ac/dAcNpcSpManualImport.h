#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x1E28C in ModuleIncSave.cro, offset_to_top 0, 89 entries
class AcNpcSpManualImport : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpManualImport(); // ctor address unknown
    virtual ~AcNpcSpManualImport(); // ModuleIncSave.cro +0x0082C0 slot 0x00
    virtual void Initialize(); // ModuleIncSave.cro +0x018DAC slot 0x0C
    virtual void Finalize(); // ModuleIncSave.cro +0x018E48 slot 0x18
    virtual void Calc(); // ModuleIncSave.cro +0x0074BC slot 0x24
    virtual void Unk0(); // ModuleIncSave.cro +0x017D10 slot 0x3C
};
