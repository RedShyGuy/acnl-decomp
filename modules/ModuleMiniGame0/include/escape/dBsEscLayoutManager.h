#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0174 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscLayoutManager : public ::UtlBase<Base>
{
public:
    BsEscLayoutManager(); // ctor address unknown
    virtual ~BsEscLayoutManager(); // ModuleMiniGame0.cro +0x078904 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x078754 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x078640 slot 0x30
};
} // namespace escape
