#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace esc {
// vtable +0xDEA84 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscModelBase : public ::UtlBase<Base>
{
public:
    BsEscModelBase(); // ctor address unknown
    virtual ~BsEscModelBase(); // ModuleMiniGame0.cro +0x070460 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x01179C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0116F4 slot 0x30
};
} // namespace esc
