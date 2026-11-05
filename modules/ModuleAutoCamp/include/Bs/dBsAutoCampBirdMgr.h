#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0xDED8 in ModuleAutoCamp.cro, offset_to_top 0, 16 entries
class BsAutoCampBirdMgr : public ::Base
{
public:
    BsAutoCampBirdMgr(); // ctor address unknown
    virtual ~BsAutoCampBirdMgr(); // ModuleAutoCamp.cro +0x009D5C slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleAutoCamp.cro +0x009988 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleAutoCamp.cro +0x009C70 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleAutoCamp.cro +0x009B20 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleAutoCamp.cro +0x009980 slot 0x30
    virtual bool Unk0(); // ModuleAutoCamp.cro +0x00B590 slot 0x3C
};
