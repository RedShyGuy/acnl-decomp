#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE18CC in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsEscapeFishingManager : public ::UtlBase<Base>
{
public:
    BsEscapeFishingManager(); // ctor address unknown
    virtual ~BsEscapeFishingManager(); // ModuleMiniGame0.cro +0x08DD70 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x08DCE4 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x08CE94 slot 0x30
};
} // namespace escape
