#pragma once

#include "decomp.h"
#include "script/dWordFix.h"

struct SvMyDesign;

// the words of a design: its title, its creator and the town of the creator (game code, not decompiled yet; names are ours)
struct DesignWords
{
    DesignWords(const SvMyDesign* design); // 0x0056E0C4
    ~DesignWords() {}

    script::WordFix<21> title;   // 0x00
    script::WordFix<9> creator;  // 0x44
    script::WordFix<9> town;     // 0x70
};
