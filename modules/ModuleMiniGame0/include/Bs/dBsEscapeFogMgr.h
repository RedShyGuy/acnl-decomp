#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0xDE6C4 in ModuleMiniGame0.cro, offset_to_top 0, 16 entries
class BsEscapeFogMgr : public ::Base
{
public:
    BsEscapeFogMgr(); // ctor address unknown
    virtual ~BsEscapeFogMgr(); // ModuleMiniGame0.cro +0x0017F4 slot 0x00
    virtual void Initialize(); // ModuleMiniGame0.cro +0x0016CC slot 0x0C
    virtual void Finalize(); // ModuleMiniGame0.cro +0x001720 slot 0x18
    virtual void Calc(); // ModuleMiniGame0.cro +0x001700 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0016A8 slot 0x30
    virtual void Unk0(); // ModuleMiniGame0.cro +0x0B6974 slot 0x3C
};
