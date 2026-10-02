#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0xDED8 in ModuleAutoCamp.cro, offset_to_top 0, 16 entries
class BsAutoCampBirdMgr : public ::Base
{
public:
    BsAutoCampBirdMgr(); // ctor address unknown
    virtual ~BsAutoCampBirdMgr(); // ModuleAutoCamp.cro +0x009D5C slot 0x00
    virtual void Initialize(); // ModuleAutoCamp.cro +0x009988 slot 0x0C
    virtual void Finalize(); // ModuleAutoCamp.cro +0x009C70 slot 0x18
    virtual void Calc(); // ModuleAutoCamp.cro +0x009B20 slot 0x24
    virtual void Draw(); // ModuleAutoCamp.cro +0x009980 slot 0x30
    virtual void Unk0(); // ModuleAutoCamp.cro +0x00B590 slot 0x3C
};
