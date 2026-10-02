#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x14034 in ModuleClub.cro, offset_to_top 0, 89 entries
class AcNpcSpTotakeke : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpTotakeke(); // ctor address unknown
    virtual ~AcNpcSpTotakeke(); // ModuleClub.cro +0x008208 slot 0x00
    virtual void Calc(); // ModuleClub.cro +0x007C70 slot 0x24
};
