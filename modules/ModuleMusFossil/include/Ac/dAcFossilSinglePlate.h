#pragma once

#include "decomp.h"
#include "Ac/dAcSimpleTalk.h"
#include "Other/dFossilSinglePlateTalkRecept.h"

// vtable +0x2478 in ModuleMusFossil.cro, offset_to_top 0, 37 entries
class AcFossilSinglePlate : public ::AcSimpleTalk<FossilSinglePlateTalkRecept>
{
public:
    AcFossilSinglePlate(); // ctor address unknown
    virtual ~AcFossilSinglePlate(); // ModuleMusFossil.cro +0x000B08 slot 0x00
};
