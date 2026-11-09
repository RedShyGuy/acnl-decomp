#include "script/dIWord.h"

namespace script {
// 0x005EB0A0 slot 0x24
WordInfo* IWord::GetInfo()
{
    return NULL;
}

// 0x0075C8BC slot 0x28
const WordInfo* IWord::GetInfo() const
{
    return NULL;
}

// 0x0075C8C4 slot 0x2C
bool IWord::IsSv() const
{
    return false;
}
} // namespace script
