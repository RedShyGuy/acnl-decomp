#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x66E58 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpFortuneteller : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpFortuneteller(); // ctor address unknown
    virtual ~AcNpcSpFortuneteller(); // ModuleIndoor.cro +0x03B6C4 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x03B1F0 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x03B124 slot 0x30
};
