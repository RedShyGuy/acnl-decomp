#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0xDABC in ModuleAutoCamp.cro, offset_to_top 0, 24 entries
class AcAutoCampBird : public ::UtlBase<Actor>
{
public:
    AcAutoCampBird(); // ctor address unknown
    virtual ~AcAutoCampBird(); // ModuleAutoCamp.cro +0x004108 slot 0x00
    virtual void Initialize(); // ModuleAutoCamp.cro +0x00C1C4 slot 0x0C
    virtual void Finalize(); // ModuleAutoCamp.cro +0x00C260 slot 0x18
    virtual void Calc(); // ModuleAutoCamp.cro +0x003E18 slot 0x24
    virtual void Draw(); // ModuleAutoCamp.cro +0x003D9C slot 0x30
    virtual void Unk0(); // ModuleAutoCamp.cro +0x00B5C8 slot 0x3C
};
