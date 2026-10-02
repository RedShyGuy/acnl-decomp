#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x46FC in ModuleSummer.cro, offset_to_top 0, 89 entries
class AcNpcSpPumpkingPre : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPumpkingPre(); // ctor address unknown
    virtual ~AcNpcSpPumpkingPre(); // ModuleSummer.cro +0x003768 slot 0x00
};
