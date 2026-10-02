#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDFB14 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventBase : public ::UtlBase<Base>
{
public:
    BsEscapeEventBase(); // ctor address unknown
    virtual ~BsEscapeEventBase(); // ModuleMiniGame0.cro +0x0431CC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x04319C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x043064 slot 0x30
};
} // namespace escape
