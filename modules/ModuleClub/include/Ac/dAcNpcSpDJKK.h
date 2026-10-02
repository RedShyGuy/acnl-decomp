#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13BA0 in ModuleClub.cro, offset_to_top 0, 89 entries
class AcNpcSpDJKK : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpDJKK(); // ctor address unknown
    virtual ~AcNpcSpDJKK(); // ModuleClub.cro +0x001C34 slot 0x00
    virtual void Initialize(); // ModuleClub.cro +0x011D4C slot 0x0C
    virtual void Finalize(); // ModuleClub.cro +0x011DE8 slot 0x18
    virtual void Unk0(); // ModuleClub.cro +0x0110C8 slot 0x3C
};
