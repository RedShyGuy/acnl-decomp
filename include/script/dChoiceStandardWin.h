#pragma once

#include "decomp.h"
#include "script/dChoiceStandard.h"

namespace script {
// RTTI N6script17ChoiceStandardWinE @ 0x008D3974
// vtable 0x0090A34C (vptr 0x0090A354), offset_to_top 0, 15 entries
class ChoiceStandardWin : public ::script::ChoiceStandard
{
public:
    ChoiceStandardWin(); // ctor address unknown
    virtual ~ChoiceStandardWin(); // 0x005E8574 slot 0x00 | slot vf_0x00 of script::ChoiceStandard
    // 0x005E8564 slot 0x04 | slot vf_0x04 of script::ChoiceStandard (deleting dtor)
    virtual void vf_0x10(); // 0x007131D4 slot 0x10 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x18(); // 0x0075C18C slot 0x18 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x20(); // 0x0075C1E0 slot 0x20 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x24(); // 0x00724208 slot 0x24 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x30(); // 0x005E83F4 slot 0x30 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x34(); // 0x0020A1E4 slot 0x34 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x38(); // 0x0075C178 slot 0x38 | virtual slot, introduced by script::ChoiceStandard
};
} // namespace script
