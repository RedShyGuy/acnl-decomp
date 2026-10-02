#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0AF8 in ModuleMiniGame0.cro, offset_to_top 0, 25 entries
class BsEscapeEventManager : public ::UtlBase<Base>
{
public:
    BsEscapeEventManager(); // ctor address unknown
    virtual ~BsEscapeEventManager(); // ModuleMiniGame0.cro +0x080C44 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x080BBC slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x080AC0 slot 0x30
};
} // namespace escape
