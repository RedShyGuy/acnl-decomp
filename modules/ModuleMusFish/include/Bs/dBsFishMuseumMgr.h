#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x2EA9C in ModuleMusFish.cro, offset_to_top 0, 16 entries
class BsFishMuseumMgr : public ::Base
{
public:
    BsFishMuseumMgr(); // ctor address unknown
    virtual ~BsFishMuseumMgr(); // ModuleMusFish.cro +0x00DBDC slot 0x00
    virtual void Initialize(); // ModuleMusFish.cro +0x00D540 slot 0x0C
    virtual void Finalize(); // ModuleMusFish.cro +0x00D9B4 slot 0x18
    virtual void Calc(); // ModuleMusFish.cro +0x00D820 slot 0x24
};
