#include "script/dWordCPtr.h"

namespace script {
// 0x005FEA70 slot 0x00
// 0x005FEA6C (deleting dtor)
WordCPtr::~WordCPtr()
{
    // empty (in the original too)
}

// 0x005EB12C slot 0x08
char16* WordCPtr::GetWord()
{
    return NULL;
}

// 0x0075DD28 slot 0x0C
const char16* WordCPtr::GetWord() const
{
    return m_pWord;
}

// 0x0075DD20 slot 0x10
u32 WordCPtr::GetSize() const
{
    return m_Size;
}

// 0x005EB120 slot 0x14
void WordCPtr::Copy(const IWord&)
{
    // empty (in the original too)
}

// 0x005EB11C slot 0x18
void WordCPtr::Append(const IWord&)
{
    // empty (in the original too)
}

// 0x005EB128 slot 0x1C
void WordCPtr::Clear()
{
    // empty (in the original too)
}

// 0x005EB124 slot 0x20
void WordCPtr::Fill(char16, u32)
{
    // empty (in the original too)
}
} // namespace script
