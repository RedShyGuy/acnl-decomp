#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace esc {
// vtable +0xDE9F0 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscUkiModel : public ::esc::BsEscModelBase
{
public:
    BsEscUkiModel(); // ctor address unknown
    virtual ~BsEscUkiModel(); // ModuleMiniGame0.cro +0x0112F0 slot 0x00
};
} // namespace esc
