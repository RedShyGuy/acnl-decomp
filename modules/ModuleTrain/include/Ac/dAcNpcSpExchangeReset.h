#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xF44C in ModuleTrain.cro, offset_to_top 0, 89 entries
class AcNpcSpExchangeReset : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpExchangeReset(); // ctor address unknown
    virtual ~AcNpcSpExchangeReset(); // ModuleTrain.cro +0x009D78 slot 0x00
    virtual void Calc(); // ModuleTrain.cro +0x009B8C slot 0x24
};
