#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"

// vtable +0x1FBD4 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x20D98 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x22308 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x254BC in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x293A4 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2F4D0 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x20200 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x213C4 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x22934 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x25AE8 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x299D0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F59C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F5F8 in ModuleMusFish.cro, offset_to_top -596, 11 entries
// vtable +0x29998 in ModuleMusFish.cro, offset_to_top -768, 11 entries
// vtable +0x2138C in ModuleMusFish.cro, offset_to_top -780, 11 entries
// vtable +0x228FC in ModuleMusFish.cro, offset_to_top -780, 11 entries
// vtable +0x201C8 in ModuleMusFish.cro, offset_to_top -788, 11 entries
// vtable +0x25AB0 in ModuleMusFish.cro, offset_to_top -896, 11 entries
class AcFishMuseumRiver : public ::AcFishMuseumBase
{
public:
    AcFishMuseumRiver(); // ctor address unknown
    virtual ~AcFishMuseumRiver(); // ModuleMusFish.cro +0x0154C0 slot 0x00
};
