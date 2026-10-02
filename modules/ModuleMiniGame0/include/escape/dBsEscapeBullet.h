#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF31C in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeBullet : public ::UtlBase<Base>
{
public:
    BsEscapeBullet(); // ctor address unknown
    virtual ~BsEscapeBullet(); // ModuleMiniGame0.cro +0x02AF6C slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x02AE84 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x02AB2C slot 0x30
};
} // namespace escape
