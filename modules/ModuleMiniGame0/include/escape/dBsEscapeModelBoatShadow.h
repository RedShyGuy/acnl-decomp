#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xE1E10 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeModelBoatShadow : public ::esc::BsEscModelBase
{
public:
    BsEscapeModelBoatShadow(); // ctor address unknown
    virtual ~BsEscapeModelBoatShadow(); // ModuleMiniGame0.cro +0x093BD4 slot 0x00
};
} // namespace escape
