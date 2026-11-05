#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0xDE70C in ModuleMiniGame0.cro, offset_to_top 0, 25 entries
class BsMiniGame0Mgr : public ::UtlBase<Base>
{
public:
    class NfpTagWork;
    BsMiniGame0Mgr(); // ctor address unknown
    virtual ~BsMiniGame0Mgr(); // ModuleMiniGame0.cro +0x005A68 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleMiniGame0.cro +0x0D79D8 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleMiniGame0.cro +0x0D7A74 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleMiniGame0.cro +0x004900 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleMiniGame0.cro +0x0048A8 slot 0x30
    virtual bool Unk0(); // ModuleMiniGame0.cro +0x0B7470 slot 0x3C
};
