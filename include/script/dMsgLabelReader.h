#pragma once

#include "decomp.h"

namespace script {
class WordPtr;

// The game object behind the pointer 0x0095EEEC that reads the text of a message label into a word.
// (Game code that is not decompiled yet; the class and all names are ours.)
class MsgLabelReader
{
public:
    void Begin(void* unknown); // 0x005EB3C8
    // the text of label into word (dNetCec passes kind 3); false if it is not there
    bool ReadWord(WordPtr* word, const char* label, u32 kind, u32 unknown); // 0x005EB434
    void End(); // 0x005EB3AC

    static MsgLabelReader* s_pInstance;
};
} // namespace script
