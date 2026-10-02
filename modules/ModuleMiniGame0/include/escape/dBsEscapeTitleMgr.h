#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF958 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeTitleMgr : public ::UtlBase<Base>
{
public:
    BsEscapeTitleMgr(); // ctor address unknown
    virtual ~BsEscapeTitleMgr(); // ModuleMiniGame0.cro +0x03A708 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x03A168 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x03A088 slot 0x30
    virtual void OnNotify(); // ModuleMiniGame0.cro +0x03594C slot 0x38
};
} // namespace escape
