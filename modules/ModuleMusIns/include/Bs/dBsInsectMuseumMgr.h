#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x2206C in ModuleMusIns.cro, offset_to_top 0, 16 entries
class BsInsectMuseumMgr : public ::Base
{
public:
    BsInsectMuseumMgr(); // ctor address unknown
    virtual ~BsInsectMuseumMgr(); // ModuleMusIns.cro +0x0100A4 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleMusIns.cro +0x00FC30 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleMusIns.cro +0x00FF50 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleMusIns.cro +0x00FF04 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleMusIns.cro +0x00FC28 slot 0x30
    virtual bool Unk0(); // ModuleMusIns.cro +0x011F20 slot 0x3C
};
