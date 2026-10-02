#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"

// vtable +0x2073C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2296C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x245F4 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x25B20 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x29A08 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2E18C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x20D60 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x22F90 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x24C18 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x26144 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2A02C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E250 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E2AC in ModuleMusFish.cro, offset_to_top -632, 11 entries
// vtable +0x29FF4 in ModuleMusFish.cro, offset_to_top -808, 11 entries
// vtable +0x24BE0 in ModuleMusFish.cro, offset_to_top -820, 11 entries
// vtable +0x2610C in ModuleMusFish.cro, offset_to_top -820, 11 entries
// vtable +0x20D28 in ModuleMusFish.cro, offset_to_top -824, 11 entries
// vtable +0x22F58 in ModuleMusFish.cro, offset_to_top -892, 11 entries
class AcFishMuseumSea : public ::AcFishMuseumBase
{
public:
    AcFishMuseumSea(); // ctor address unknown
    virtual ~AcFishMuseumSea(); // ModuleMusFish.cro +0x00A550 slot 0x00
};
