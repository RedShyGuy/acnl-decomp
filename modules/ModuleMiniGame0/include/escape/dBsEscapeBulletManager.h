#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0ECC in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsEscapeBulletManager : public ::UtlBase<Base>
{
public:
    BsEscapeBulletManager(); // ctor address unknown
    virtual ~BsEscapeBulletManager(); // ModuleMiniGame0.cro +0x082CEC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x082BA8 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x08284C slot 0x30
};
} // namespace escape
