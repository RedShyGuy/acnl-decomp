#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x2FA28 in ModuleMusFish.cro, offset_to_top 0, 16 entries
class BsSealifeMuseumMgr : public ::Base
{
public:
    BsSealifeMuseumMgr(); // ctor address unknown
    virtual ~BsSealifeMuseumMgr(); // ModuleMusFish.cro +0x016E80 slot 0x00
    virtual void Initialize(); // ModuleMusFish.cro +0x016960 slot 0x0C
    virtual void Finalize(); // ModuleMusFish.cro +0x016D1C slot 0x18
    virtual void Calc(); // ModuleMusFish.cro +0x016CE8 slot 0x24
};
