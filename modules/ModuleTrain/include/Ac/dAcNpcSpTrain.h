#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xF008 in ModuleTrain.cro, offset_to_top 0, 89 entries
class AcNpcSpTrain : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpTrain(); // ctor address unknown
    virtual ~AcNpcSpTrain(); // ModuleTrain.cro +0x0038F0 slot 0x00
    virtual void Calc(); // ModuleTrain.cro +0x002FD8 slot 0x24
};
