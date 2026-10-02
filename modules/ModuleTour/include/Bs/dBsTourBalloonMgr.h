#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x107D4 in ModuleTour.cro, offset_to_top 0, 22 entries
class BsTourBalloonMgr : public ::UtlBase<Base>
{
public:
    BsTourBalloonMgr(); // ctor address unknown
    virtual ~BsTourBalloonMgr(); // ModuleTour.cro +0x006304 slot 0x00
    virtual void Initialize(); // ModuleTour.cro +0x00E868 slot 0x0C
    virtual void Finalize(); // ModuleTour.cro +0x00E904 slot 0x18
    virtual void Calc(); // ModuleTour.cro +0x0060CC slot 0x24
    virtual void Draw(); // ModuleTour.cro +0x0060C4 slot 0x30
};
