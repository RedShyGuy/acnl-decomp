#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x134E8 in ModuleDream.cro, offset_to_top 0, 89 entries
class AcNpcSpSeiichi : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSeiichi(); // ctor address unknown
    virtual ~AcNpcSpSeiichi(); // ModuleDream.cro +0x0070EC slot 0x00
};
