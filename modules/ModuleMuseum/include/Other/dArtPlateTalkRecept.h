#pragma once

#include "decomp.h"
#include "Other/dMuseumPlateBaseTalkRecept.h"

// vtable +0x3480 in ModuleMuseum.cro, offset_to_top 0, 72 entries
// vtable +0x35A8 in ModuleMuseum.cro, offset_to_top -124, 14 entries
class ArtPlateTalkRecept : public ::MuseumPlateBaseTalkRecept
{
public:
    ArtPlateTalkRecept(); // ctor address unknown
    virtual ~ArtPlateTalkRecept(); // ModuleMuseum.cro +0x001D38 slot 0x00
};
