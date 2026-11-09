#pragma once

#include "decomp.h"
#include "script/dMsgEngineModify.h"
#include "script/dIWord.h"

namespace script {
// replaces a character of a text by another (member names are ours)
// RTTI N6script13ReplaceEngineE @ 0x008D387C
// vtable 0x0090A180 (vptr 0x0090A188), offset_to_top 0, 7 entries
class ReplaceEngine : public ::script::MsgEngineModify
{
public:
    ReplaceEngine(); // 0x005E45B4 (name after the RTTI)
    virtual ~ReplaceEngine(); // 0x005E45E4 slot 0x00
    // 0x005E45E0 slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x005E44F4 slot 0x14

    // whether a character was replaced
    bool Replace(IWord* word, char16 find, char16 replace); // 0x005E4524 (name is ours)

    char16 m_Find;     // 0x0E
    char16 m_Replace;  // 0x10
};
ASSERT_SIZE(ReplaceEngine, 0x14);
} // namespace script
