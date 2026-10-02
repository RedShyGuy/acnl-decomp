#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE1120 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventMakeTool : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventMakeTool(); // ctor address unknown
    virtual ~BsEscapeEventMakeTool(); // ModuleMiniGame0.cro +0x08432C slot 0x00
};
} // namespace escape
