#include "script/dCountCode.h"

namespace script {
// 0x0060081C slot 0x14
void CountCode::OnChar(const CharInfo* info)
{
    if (!m_IsCountReturn && info->units == '\n') {
        return;
    }
    m_Count++;
}

// 0x00600840 slot 0x18
void CountCode::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x00600854 slot 0x00
// 0x00600844 (deleting dtor)
CountCode::~CountCode()
{
    // (only the base is destroyed)
}
} // namespace script
