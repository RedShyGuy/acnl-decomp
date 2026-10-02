#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x13D0C in ModuleClub.cro, offset_to_top 0, 91 entries
class AcNpcSpSisyo : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSisyo(); // ctor address unknown
    virtual ~AcNpcSpSisyo(); // ModuleClub.cro +0x0035C4 slot 0x00
    virtual void Calc(); // ModuleClub.cro +0x0031B8 slot 0x24
    virtual void Draw(); // ModuleClub.cro +0x0031B4 slot 0x30
};
