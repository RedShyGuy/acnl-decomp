#pragma once

#include "decomp.h"
#include "Ac/dAcSimpleTalk.h"
#include "Other/dFossilPartsPlateTalkRecept.h"

// vtable +0x23DC in ModuleMusFossil.cro, offset_to_top 0, 37 entries
class AcFossilPartsPlate : public ::AcSimpleTalk<FossilPartsPlateTalkRecept>
{
public:
    AcFossilPartsPlate(); // ctor address unknown
    virtual ~AcFossilPartsPlate(); // ModuleMusFossil.cro +0x000A9C slot 0x00
    virtual void Initialize(); // ModuleMusFossil.cro +0x001178 slot 0x0C
    virtual void Finalize(); // ModuleMusFossil.cro +0x001214 slot 0x18
    virtual void Unk0(); // ModuleMusFossil.cro +0x000E98 slot 0x3C
};
