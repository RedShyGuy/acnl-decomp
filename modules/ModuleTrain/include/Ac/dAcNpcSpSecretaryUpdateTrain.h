#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xF73C in ModuleTrain.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryUpdateTrain : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryUpdateTrain(); // ctor address unknown
    virtual ~AcNpcSpSecretaryUpdateTrain(); // ModuleTrain.cro +0x00BE2C slot 0x00
    virtual void Calc(); // ModuleTrain.cro +0x00B0B8 slot 0x24
};
