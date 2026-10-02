#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "sead/hostio/seadNode.h"

// vtable +0xF5B8 in ModuleTrain.cro, offset_to_top 0, 89 entries
// vtable +0xF724 in ModuleTrain.cro, offset_to_top -7636, 1 entries
class AcNpcSpExchangeMaster : public ::AcNpcSp, public ::sead::hostio::Node
{
public:
    class TalkRecept;
    AcNpcSpExchangeMaster(); // ctor address unknown
    virtual ~AcNpcSpExchangeMaster(); // ModuleTrain.cro +0x00AA98 slot 0x00
    virtual void Initialize(); // ModuleTrain.cro +0x00D16C slot 0x0C
    virtual void Finalize(); // ModuleTrain.cro +0x00D208 slot 0x18
    virtual void Calc(); // ModuleTrain.cro +0x00A7D8 slot 0x24
    virtual void Unk0(); // ModuleTrain.cro +0x00C074 slot 0x3C
};
