#pragma once

#include "decomp.h"
#include "script/dChoiceStandard.h"

namespace script {
// vtable +0xE5008 in ModuleMiniGame0.cro, offset_to_top 0, 15 entries
class EscapeChoiceStandardWin : public ::script::ChoiceStandard
{
public:
    EscapeChoiceStandardWin(); // ctor address unknown
    virtual ~EscapeChoiceStandardWin(); // ModuleMiniGame0.cro +0x05300C slot 0x00
};
} // namespace script
