#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x1427C in ModuleClub.cro, offset_to_top 0, 22 entries
class BsClub444ViewMgr : public ::UtlBase<Base>
{
public:
    BsClub444ViewMgr(); // ctor address unknown
    virtual ~BsClub444ViewMgr(); // ModuleClub.cro +0x00F670 slot 0x00
    virtual void Initialize(); // ModuleClub.cro +0x011A24 slot 0x0C
    virtual void Finalize(); // ModuleClub.cro +0x011AC0 slot 0x18
    virtual void Calc(); // ModuleClub.cro +0x00EF40 slot 0x24
    virtual void Draw(); // ModuleClub.cro +0x00EEFC slot 0x30
    virtual void Unk0(); // ModuleClub.cro +0x011098 slot 0x3C
};
