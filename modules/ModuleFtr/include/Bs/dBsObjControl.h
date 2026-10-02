#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x2733C in ModuleFtr.cro, offset_to_top 0, 16 entries
class BsObjControl : public ::Base
{
public:
    BsObjControl(); // ctor address unknown
    virtual ~BsObjControl(); // ModuleFtr.cro +0x00991C slot 0x00
    virtual void Initialize(); // ModuleFtr.cro +0x008E24 slot 0x0C
    virtual void Finalize(); // ModuleFtr.cro +0x009624 slot 0x18
    virtual void Calc(); // ModuleFtr.cro +0x009614 slot 0x24
    virtual void Draw(); // ModuleFtr.cro +0x008E1C slot 0x30
    virtual void Unk0(); // ModuleFtr.cro +0x01EB6C slot 0x3C
};
