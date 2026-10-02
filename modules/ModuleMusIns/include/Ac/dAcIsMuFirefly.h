#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklMat.h"

// vtable +0x20810 in ModuleMusIns.cro, offset_to_top 0, 47 entries
// vtable +0x208D8 in ModuleMusIns.cro, offset_to_top -396, 9 entries
// vtable +0x20904 in ModuleMusIns.cro, offset_to_top -476, 2 entries
// vtable +0x2093C in ModuleMusIns.cro, offset_to_top -804, 11 entries
class AcIsMuFirefly : public ::AcInsectMuseumBase, public ::ResourceGetSklMat, public ::ObjectState<AcIsMuFirefly>
{
public:
    AcIsMuFirefly(); // ctor address unknown
    virtual ~AcIsMuFirefly(); // ModuleMusIns.cro +0x006768 slot 0x00
};
