#pragma once

#include "decomp.h"

namespace script {
// RTTI N6script5IWordE @ 0x008D3A70
class IWord
{
public:
    IWord() {}

    // the number of characters before the end of the text (name is ours)
    u32 GetLength() const; // 0x0075C7B4
};
} // namespace script
