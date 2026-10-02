#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetMaterial.h"

// vtable +0x223A0 in ModuleMusIns.cro, offset_to_top 0, 47 entries
// vtable +0x22468 in ModuleMusIns.cro, offset_to_top -396, 7 entries
// vtable +0x2248C in ModuleMusIns.cro, offset_to_top -448, 2 entries
// vtable +0x224C4 in ModuleMusIns.cro, offset_to_top -728, 11 entries
class AcIsMuAnt : public ::AcInsectMuseumBase, public ::ResourceGetMaterial, public ::ObjectState<AcIsMuAnt>
{
public:
    AcIsMuAnt(); // ctor address unknown
    virtual ~AcIsMuAnt(); // ModuleMusIns.cro +0x011200 slot 0x00
};
