#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDFAB4 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscPauseManager : public ::UtlBase<Base>
{
public:
    BsEscPauseManager(); // ctor address unknown
    virtual ~BsEscPauseManager(); // ModuleMiniGame0.cro +0x03FA24 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x03F978 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x03F628 slot 0x30
};
} // namespace escape
