#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x4424 in ModuleSummer.cro, offset_to_top 0, 89 entries
class AcNpcSpPyontarou : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPyontarou(); // ctor address unknown
    virtual ~AcNpcSpPyontarou(); // ModuleSummer.cro +0x001E10 slot 0x00
};
