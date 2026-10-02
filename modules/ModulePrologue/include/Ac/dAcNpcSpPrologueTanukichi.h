#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x88A8 in ModulePrologue.cro, offset_to_top 0, 89 entries
class AcNpcSpPrologueTanukichi : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPrologueTanukichi(); // ctor address unknown
    virtual ~AcNpcSpPrologueTanukichi(); // ModulePrologue.cro +0x0057FC slot 0x00
    virtual void Initialize(); // ModulePrologue.cro +0x007110 slot 0x0C
    virtual void Finalize(); // ModulePrologue.cro +0x0071AC slot 0x18
    virtual void Unk0(); // ModulePrologue.cro +0x006C0C slot 0x3C
};
