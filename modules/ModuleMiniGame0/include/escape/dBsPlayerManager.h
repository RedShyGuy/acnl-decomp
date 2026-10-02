#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF6B8 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsPlayerManager : public ::UtlBase<Base>
{
public:
    BsPlayerManager(); // ctor address unknown
    virtual ~BsPlayerManager(); // ModuleMiniGame0.cro +0x033310 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x032B14 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0325B4 slot 0x30
};
} // namespace escape
