#pragma once

#include "decomp.h"
#include "script/dWordFix.h"

namespace script {
// a word for a number (member names are ours)
// RTTI N6script12WordNumDigitE @ 0x008D3840
// vtable 0x0090A0F8 (vptr 0x0090A100), offset_to_top 0, 12 entries
class WordNumDigit : public ::script::WordFix<19u>
{
public:
    WordNumDigit(); // 0x00312C58 (name after the RTTI)
    virtual ~WordNumDigit(); // 0x00312D38 slot 0x00 | slot vf_0x00 of script::WordPtr
    // 0x005E3E04 slot 0x04 (deleting dtor)
    virtual void Clear(); // 0x005E3DD4 slot 0x1C
    // (game code, not decompiled yet; name is ours) the number is formatted (three values of the format)
    void Func312CD4(s32 value, u32 a, u32 b, u32 c); // 0x00312CD4

    u8 m_Kind;   // 0x3E how the number was formatted (Insert: 1 number, 2 hour, 3 minute, 6/7 decimal, 0 by the recept)
    s32 m_Value;  // 0x40 the number
};
ASSERT_SIZE(WordNumDigit, 0x44);
} // namespace script
