#include "script/dReplaceEngine.h"

namespace script {
// 0x005E44F4 slot 0x14
void ReplaceEngine::OnChar(const CharInfo* info)
{
    if (m_Find == info->units && m_pChar != NULL) {
        *m_pChar = m_Replace;
        m_IsModified = true;
    }
}

// 0x005E4524 (name is ours)
bool ReplaceEngine::Replace(IWord* word, char16 find, char16 replace)
{
    m_IsModified = false;
    m_Find = find;
    m_Replace = replace;
    if (word != NULL) {
        char16* text = word->GetWord();
        u32 size = word->GetSize();
        if (text != NULL && size != 0 && *text != 0) {
            SetText(text);
            Run(&s_pNoStop);
        }
    }
    return m_IsModified;
}

// 0x005E45B4 (name after the RTTI)
ReplaceEngine::ReplaceEngine() : m_Find(0), m_Replace(0)
{
}

// 0x005E45E4 slot 0x00
// 0x005E45E0 (deleting dtor)
ReplaceEngine::~ReplaceEngine()
{
    // empty (in the original too)
}
} // namespace script
