#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x12F78 in ModuleRealVillage.cro, offset_to_top 0, 89 entries
class AcNpcSpCleaning : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpCleaning(); // ctor address unknown
    virtual ~AcNpcSpCleaning(); // ModuleRealVillage.cro +0x003C7C slot 0x00
    virtual void Initialize(); // ModuleRealVillage.cro +0x0117B8 slot 0x0C
    virtual void Finalize(); // ModuleRealVillage.cro +0x011854 slot 0x18
    virtual void Unk0(); // ModuleRealVillage.cro +0x010E1C slot 0x3C
};
