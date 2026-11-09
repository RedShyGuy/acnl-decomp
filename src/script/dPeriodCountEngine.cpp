#include "script/dPeriodCountEngine.h"

namespace script {
// 0x005E879C slot 0x14
void PeriodCountEngine::OnChar(const CharInfo* info)
{
    if (info->units == '.') {
        m_Count++;
    } else {
        m_IsStop = 1;
    }
}

// 0x005E87BC slot 0x18
void PeriodCountEngine::OnTag(const Tag* tag)
{
    if (tag->group != 7 || tag->index != 0) {
        m_IsStop = 1;
    }
}

// 0x005E87D8 slot 0x10
bool PeriodCountEngine::IsContinue()
{
    return m_IsStop == 0;
}

// 0x005E87FC slot 0x00
// 0x005E87EC (deleting dtor)
PeriodCountEngine::~PeriodCountEngine()
{
    // (only the base is destroyed)
}
} // namespace script
