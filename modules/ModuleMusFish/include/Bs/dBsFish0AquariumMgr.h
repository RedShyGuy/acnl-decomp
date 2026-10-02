#pragma once

#include "decomp.h"
#include "Bs/dBsAquariumMgr.h"

// vtable +0x2F9C8 in ModuleMusFish.cro, offset_to_top 0, 22 entries
class BsFish0AquariumMgr : public ::BsAquariumMgr
{
public:
    BsFish0AquariumMgr(); // ctor address unknown
    virtual ~BsFish0AquariumMgr(); // ModuleMusFish.cro +0x00EBFC slot 0x00
};
