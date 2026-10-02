#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace minigame0 {
// vtable +0xE55C8 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsMiniGame0Bg : public ::UtlBase<Base>
{
public:
    BsMiniGame0Bg(); // ctor address unknown
    virtual ~BsMiniGame0Bg(); // ModuleMiniGame0.cro +0x0B0DDC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x0B0D7C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0B0D58 slot 0x30
};
} // namespace minigame0
