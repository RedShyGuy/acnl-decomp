#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF5CC in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeGameMgr : public ::UtlBase<Base>
{
public:
    BsEscapeGameMgr(); // ctor address unknown
    virtual ~BsEscapeGameMgr(); // ModuleMiniGame0.cro +0x02FD40 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x02FB5C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x02F658 slot 0x30
    virtual void OnNotify(); // ModuleMiniGame0.cro +0x02F078 slot 0x38
};
} // namespace escape
