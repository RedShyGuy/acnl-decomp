#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x3320 in ModuleMuseum.cro, offset_to_top 0, 23 entries
class AcRobjPoster : public ::UtlBase<Actor>
{
public:
    AcRobjPoster(); // ctor address unknown
    virtual ~AcRobjPoster(); // ModuleMuseum.cro +0x001E60 slot 0x00
    virtual void Initialize(); // ModuleMuseum.cro +0x002AA0 slot 0x0C
    virtual void Finalize(); // ModuleMuseum.cro +0x002B3C slot 0x18
    virtual void Calc(); // ModuleMuseum.cro +0x001334 slot 0x24
    virtual void Draw(); // ModuleMuseum.cro +0x00131C slot 0x30
};
