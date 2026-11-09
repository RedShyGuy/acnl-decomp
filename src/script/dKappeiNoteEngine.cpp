#include "script/dKappeiNoteEngine.h"

namespace script {
// 0x005E72AC slot 0x14
void KappeiNoteEngine::OnChar(const CharInfo* info)
{
    u32 c = info->units;
    bool isReturn = c == '\n';
    bool isSpace = c == ' ' || c == 0x3000;
    if (!(isSpace | isReturn)) {
        m_Count++;
    }
}

// 0x005E72E4 slot 0x18
void KappeiNoteEngine::OnTag(const Tag* tag)
{
    if (tag->group == 1) {
        m_Count++;
    } else if (tag->group == 13 && (tag->index == 6 || tag->index == 7)) {
        m_IsStop = 1;
    }
}

// 0x005E7320 slot 0x10
bool KappeiNoteEngine::IsContinue()
{
    return m_IsStop == 0;
}

// 0x005E7344 slot 0x00
// 0x005E7334 (deleting dtor)
KappeiNoteEngine::~KappeiNoteEngine()
{
    // (only the base is destroyed)
}
} // namespace script
