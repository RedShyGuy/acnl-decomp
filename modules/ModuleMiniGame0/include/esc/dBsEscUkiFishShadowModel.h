#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace esc {
// vtable +0xDEBAC in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscUkiFishShadowModel : public ::esc::BsEscModelBase
{
public:
    BsEscUkiFishShadowModel(); // ctor address unknown
    virtual ~BsEscUkiFishShadowModel(); // ModuleMiniGame0.cro +0x01208C slot 0x00
};
} // namespace esc
