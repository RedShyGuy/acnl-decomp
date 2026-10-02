#pragma once

#include "decomp.h"
#include "script/dChoiceStandard.h"

namespace script {
// RTTI N6script18ChoiceStandardItemE @ 0x008D39B0
// vtable 0x0090A3E0 (vptr 0x0090A3E8), offset_to_top 0, 15 entries
class ChoiceStandardItem : public ::script::ChoiceStandard
{
public:
    ChoiceStandardItem(); // ctor address unknown
    virtual ~ChoiceStandardItem(); // 0x005E954C slot 0x00 | slot vf_0x00 of script::ChoiceStandard
    // 0x005E953C slot 0x04 | slot vf_0x04 of script::ChoiceStandard (deleting dtor)
    virtual void vf_0x10(); // 0x0071DD68 slot 0x10 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x18(); // 0x0075C200 slot 0x18 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x20(); // 0x0075C210 slot 0x20 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x24(); // 0x0075C208 slot 0x24 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x30(); // 0x005E933C slot 0x30 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x34(); // 0x002BB0AC slot 0x34 | virtual slot, introduced by script::ChoiceStandard
    virtual void vf_0x38(); // 0x0075C1EC slot 0x38 | virtual slot, introduced by script::ChoiceStandard
};
} // namespace script
