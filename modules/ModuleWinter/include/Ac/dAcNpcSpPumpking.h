#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x12958 in ModuleWinter.cro, offset_to_top 0, 89 entries
class AcNpcSpPumpking : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPumpking(); // ctor address unknown
    virtual ~AcNpcSpPumpking(); // ModuleWinter.cro +0x0104E4 slot 0x00
    virtual void Calc(); // ModuleWinter.cro +0x01008C slot 0x24
};
