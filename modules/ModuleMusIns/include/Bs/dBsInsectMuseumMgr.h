#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x2206C in ModuleMusIns.cro, offset_to_top 0, 16 entries
class BsInsectMuseumMgr : public ::Base
{
public:
    BsInsectMuseumMgr(); // ctor address unknown
    virtual ~BsInsectMuseumMgr(); // ModuleMusIns.cro +0x0100A4 slot 0x00
    virtual void Initialize(); // ModuleMusIns.cro +0x00FC30 slot 0x0C
    virtual void Finalize(); // ModuleMusIns.cro +0x00FF50 slot 0x18
    virtual void Calc(); // ModuleMusIns.cro +0x00FF04 slot 0x24
    virtual void Draw(); // ModuleMusIns.cro +0x00FC28 slot 0x30
    virtual void Unk0(); // ModuleMusIns.cro +0x011F20 slot 0x3C
};
