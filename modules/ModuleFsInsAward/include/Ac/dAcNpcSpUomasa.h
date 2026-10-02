#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x52AC in ModuleFsInsAward.cro, offset_to_top 0, 89 entries
class AcNpcSpUomasa : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpUomasa(); // ctor address unknown
    virtual ~AcNpcSpUomasa(); // ModuleFsInsAward.cro +0x002690 slot 0x00
};
