#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x35E8 in ModuleMuseum.cro, offset_to_top 0, 72 entries
// vtable +0x3710 in ModuleMuseum.cro, offset_to_top -124, 14 entries
class MuseumPlateBaseTalkRecept : public ::ObjTalkRecept
{
public:
    MuseumPlateBaseTalkRecept(); // ctor address unknown
    virtual ~MuseumPlateBaseTalkRecept(); // ModuleMuseum.cro +0x001D3C slot 0x00
};
