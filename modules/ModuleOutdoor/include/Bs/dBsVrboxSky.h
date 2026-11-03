#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x9238C in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxSky : public ::UtlBase<Base>
{
public:
    BsVrboxSky(); // ctor address unknown
    virtual ~BsVrboxSky(); // ModuleOutdoor.cro +0x009EDC slot 0x00
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x0095BC slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x009528 slot 0x30
};
