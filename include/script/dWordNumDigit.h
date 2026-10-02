#pragma once

#include "decomp.h"
#include "script/dWordFix.h"

namespace script {
// RTTI N6script12WordNumDigitE @ 0x008D3840
// vtable 0x0090A0F8 (vptr 0x0090A100), offset_to_top 0, 12 entries
class WordNumDigit : public ::script::WordFix<19u>
{
public:
    WordNumDigit(); // ctor candidate(s) 0x00312C58 (unverified)
    virtual ~WordNumDigit(); // 0x00312D38 slot 0x00 | slot vf_0x00 of script::WordPtr
    virtual void vf_0x04(); // 0x005E3E04 slot 0x04 | virtual slot, introduced by script::WordPtr
    virtual void vf_0x1C(); // 0x005E3DD4 slot 0x1C | virtual slot, introduced by script::WordPtr
};
} // namespace script
