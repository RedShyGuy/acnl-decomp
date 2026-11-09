#include "script/dDetectCode.h"
#include "script/dCountCode.h"

namespace script {
// 0x005D5B98 (name is ours)
const char16* DetectCode::DetectFromEnd(const IWord* word, s32 index)
{
    CountCode count(true, true);
    const char16* text = word->GetWord();
    if (text != NULL) {
        count.Run(text);
    }
    s32 target = count.m_Count - index - 1;
    if (target < 0) {
        return NULL;
    }
    return Detect(word, target);
}

// 0x005D5C98 slot 0x14
void DetectCode::OnChar(const CharInfo* info)
{
    if (m_Count != m_Target) {
        m_Count++;
    } else {
        m_pFound = info->pText;
        m_IsFound = 1;
    }
}

// 0x005D5CC4 slot 0x18
void DetectCode::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x005D5CC8 slot 0x10
bool DetectCode::IsContinue()
{
    return m_IsFound == 0;
}

// 0x005D5CEC slot 0x00
// 0x005D5CDC (deleting dtor)
DetectCode::~DetectCode()
{
    // (only the base is destroyed)
}
} // namespace script
