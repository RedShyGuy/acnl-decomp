#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x12568 in ModuleWinter.cro, offset_to_top 0, 36 entries
class AcSnowBall : public ::UtlBase<DemoActor>
{
public:
    class SpWrapObjTalkRecept;
    AcSnowBall(); // ctor address unknown
    virtual ~AcSnowBall(); // ModuleWinter.cro +0x00B2D0 slot 0x00
    virtual void Calc(); // ModuleWinter.cro +0x00A4B0 slot 0x24
    virtual void Draw(); // ModuleWinter.cro +0x009E68 slot 0x30
};
