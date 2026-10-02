#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF12C in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeStage : public ::UtlBase<Base>
{
public:
    BsEscapeStage(); // ctor address unknown
    virtual ~BsEscapeStage(); // ModuleMiniGame0.cro +0x028DCC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x028394 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x027E24 slot 0x30
    virtual void OnNotify(); // ModuleMiniGame0.cro +0x0244E8 slot 0x38
};
} // namespace escape
