#pragma once

#include "decomp.h"
#include "Ac/dAcFishCommon.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1F6D0 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x1FCA0 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x20238 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x20800 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x20E64 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x213FC in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x21900 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x21E04 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x223D4 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x22A30 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x22FC8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x236E8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x23BEC in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x240F0 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x246B8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x24C50 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x25588 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x25BE4 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2617C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x26680 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x26B84 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x27088 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2758C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x27A90 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x27F94 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x28498 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2899C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x28EA0 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x29470 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x29ACC in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2A064 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2A568 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2EAE8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x1FB9C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2016C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x20704 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x20CCC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x21330 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x218C8 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x21DCC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x222D0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x228A0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x22EFC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x23494 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x23BB4 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x240B8 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x245BC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x24B84 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2511C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x25A54 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x260B0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x26648 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x26B4C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x27050 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x27554 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x27A58 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x27F5C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x28460 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x28964 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x28E68 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2936C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2993C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x29F98 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2A530 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2AA34 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2EBAC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x21D94 in ModuleMusFish.cro, offset_to_top -552, 11 entries
// vtable +0x2EC08 in ModuleMusFish.cro, offset_to_top -552, 11 entries
// vtable +0x28E30 in ModuleMusFish.cro, offset_to_top -572, 11 entries
// vtable +0x2751C in ModuleMusFish.cro, offset_to_top -576, 11 entries
// vtable +0x26B14 in ModuleMusFish.cro, offset_to_top -580, 11 entries
// vtable +0x1FB64 in ModuleMusFish.cro, offset_to_top -596, 11 entries
// vtable +0x206CC in ModuleMusFish.cro, offset_to_top -632, 11 entries
// vtable +0x24584 in ModuleMusFish.cro, offset_to_top -696, 11 entries
// vtable +0x250E4 in ModuleMusFish.cro, offset_to_top -696, 11 entries
// vtable +0x26610 in ModuleMusFish.cro, offset_to_top -708, 11 entries
// vtable +0x27018 in ModuleMusFish.cro, offset_to_top -724, 11 entries
// vtable +0x29334 in ModuleMusFish.cro, offset_to_top -728, 11 entries
// vtable +0x2345C in ModuleMusFish.cro, offset_to_top -732, 11 entries
// vtable +0x27A20 in ModuleMusFish.cro, offset_to_top -736, 11 entries
// vtable +0x21890 in ModuleMusFish.cro, offset_to_top -744, 11 entries
// vtable +0x27F24 in ModuleMusFish.cro, offset_to_top -744, 11 entries
// vtable +0x2A9FC in ModuleMusFish.cro, offset_to_top -744, 11 entries
// vtable +0x2892C in ModuleMusFish.cro, offset_to_top -748, 11 entries
// vtable +0x2A4F8 in ModuleMusFish.cro, offset_to_top -760, 11 entries
// vtable +0x22298 in ModuleMusFish.cro, offset_to_top -768, 11 entries
// vtable +0x24080 in ModuleMusFish.cro, offset_to_top -768, 11 entries
// vtable +0x29904 in ModuleMusFish.cro, offset_to_top -768, 11 entries
// vtable +0x212F8 in ModuleMusFish.cro, offset_to_top -780, 11 entries
// vtable +0x22868 in ModuleMusFish.cro, offset_to_top -780, 11 entries
// vtable +0x20134 in ModuleMusFish.cro, offset_to_top -788, 11 entries
// vtable +0x29F60 in ModuleMusFish.cro, offset_to_top -808, 11 entries
// vtable +0x23B7C in ModuleMusFish.cro, offset_to_top -812, 11 entries
// vtable +0x24B4C in ModuleMusFish.cro, offset_to_top -820, 11 entries
// vtable +0x26078 in ModuleMusFish.cro, offset_to_top -820, 11 entries
// vtable +0x20C94 in ModuleMusFish.cro, offset_to_top -824, 11 entries
// vtable +0x28428 in ModuleMusFish.cro, offset_to_top -836, 11 entries
// vtable +0x22EC4 in ModuleMusFish.cro, offset_to_top -892, 11 entries
// vtable +0x25A1C in ModuleMusFish.cro, offset_to_top -896, 11 entries
class AcFishMuseumBase : public ::AcFishCommon, public ::ResourceLoadAsyncSkeletal
{
public:
    AcFishMuseumBase(); // ctor address unknown
    virtual ~AcFishMuseumBase(); // ModuleMusFish.cro +0x013008 slot 0x00
    virtual void CanCalc() const; // ModuleMusFish.cro +0x00DC30 slot 0x20
    virtual void HandleCalcResult(oml::framework::Result); // ModuleMusFish.cro +0x012C4C slot 0x28
    virtual void Unk0(); // ModuleMusFish.cro +0x01AAF4 slot 0x3C
};
