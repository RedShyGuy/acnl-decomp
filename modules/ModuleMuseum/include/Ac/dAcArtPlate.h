#pragma once

#include "decomp.h"
#include "Ac/dAcSimpleTalk.h"
#include "Other/dArtPlateTalkRecept.h"

// vtable +0x3284 in ModuleMuseum.cro, offset_to_top 0, 37 entries
class AcArtPlate : public ::AcSimpleTalk<ArtPlateTalkRecept>
{
public:
    AcArtPlate(); // ctor address unknown
    virtual ~AcArtPlate(); // ModuleMuseum.cro +0x0004F4 slot 0x00
    virtual void Initialize(); // ModuleMuseum.cro +0x002C34 slot 0x0C
    virtual void Finalize(); // ModuleMuseum.cro +0x002CD0 slot 0x18
    virtual void Unk0(); // ModuleMuseum.cro +0x0027B8 slot 0x3C
};
