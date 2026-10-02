#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x6554 in ModulePlSelect.cro, offset_to_top 0, 89 entries
class AcNpcSpPlSelect : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPlSelect(); // ctor address unknown
    virtual ~AcNpcSpPlSelect(); // ModulePlSelect.cro +0x003DCC slot 0x00
    virtual void Initialize(); // ModulePlSelect.cro +0x004FA8 slot 0x0C
    virtual void Finalize(); // ModulePlSelect.cro +0x005044 slot 0x18
    virtual void Calc(); // ModulePlSelect.cro +0x0039A0 slot 0x24
    virtual void Unk0(); // ModulePlSelect.cro +0x0049E8 slot 0x3C
};
