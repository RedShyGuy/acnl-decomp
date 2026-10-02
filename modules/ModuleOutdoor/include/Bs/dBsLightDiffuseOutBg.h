#pragma once

#include "decomp.h"
#include "Bs/dBsLightDiffuseBase.h"

// vtable +0x9770C in ModuleOutdoor.cro, offset_to_top 0, 26 entries
class BsLightDiffuseOutBg : public ::BsLightDiffuseBase
{
public:
    BsLightDiffuseOutBg(); // ctor address unknown
    virtual ~BsLightDiffuseOutBg(); // ModuleOutdoor.cro +0x05D3BC slot 0x00
    virtual void Initialize(); // ModuleOutdoor.cro +0x05D2A8 slot 0x0C
    virtual void Finalize(); // ModuleOutdoor.cro +0x05D378 slot 0x18
    virtual void Calc(); // ModuleOutdoor.cro +0x05D358 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05D298 slot 0x30
    virtual void Unk0(); // ModuleOutdoor.cro +0x07D5E4 slot 0x3C
};
