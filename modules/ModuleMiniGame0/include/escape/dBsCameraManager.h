#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF564 in ModuleMiniGame0.cro, offset_to_top 0, 24 entries
class BsCameraManager : public ::UtlBase<Base>
{
public:
    BsCameraManager(); // ctor address unknown
    virtual ~BsCameraManager(); // ModuleMiniGame0.cro +0x02E8D4 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x02E868 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x02E844 slot 0x30
};
} // namespace escape
