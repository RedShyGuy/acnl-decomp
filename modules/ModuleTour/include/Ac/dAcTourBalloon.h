#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x10640 in ModuleTour.cro, offset_to_top 0, 23 entries
class AcTourBalloon : public ::UtlBase<Actor>
{
public:
    AcTourBalloon(); // ctor address unknown
    virtual ~AcTourBalloon(); // ModuleTour.cro +0x003E98 slot 0x00
    virtual void Initialize(); // ModuleTour.cro +0x00E9FC slot 0x0C
    virtual void Finalize(); // ModuleTour.cro +0x00EA98 slot 0x18
    virtual void Calc(); // ModuleTour.cro +0x003BD8 slot 0x24
    virtual void Draw(); // ModuleTour.cro +0x0038E0 slot 0x30
};
