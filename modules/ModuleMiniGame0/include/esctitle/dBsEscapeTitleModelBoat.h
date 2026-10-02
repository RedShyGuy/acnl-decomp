#pragma once

#include "decomp.h"
#include "esctitle/dBsEscapeTitleModel.h"

namespace esctitle {
// vtable +0xE5380 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeTitleModelBoat : public ::esctitle::BsEscapeTitleModel
{
public:
    BsEscapeTitleModelBoat(); // ctor address unknown
    virtual ~BsEscapeTitleModelBoat(); // ModuleMiniGame0.cro +0x0ABD88 slot 0x00
    virtual void Draw(); // ModuleMiniGame0.cro +0x0ABC78 slot 0x30
};
} // namespace esctitle
