#include "script/dCmpFuzzyEngine_EngineB.h"

namespace script {
// 0x005E57D8 slot 0x14
void CmpFuzzyEngine::EngineB::OnChar(const CharInfo* info)
{
    if (m_pInfo != NULL && IsFuzzyEqual(*m_pInfo->pText, *info->pText)) {
        m_IsMatch = true;
    }
    m_IsContinue = false;
}

// 0x005E58EC slot 0x18
void CmpFuzzyEngine::EngineB::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x005E58F0 slot 0x10
bool CmpFuzzyEngine::EngineB::IsContinue()
{
    return m_IsContinue;
}

// 0x005E5908 slot 0x00
// 0x005E58F8 (deleting dtor)
CmpFuzzyEngine::EngineB::~EngineB()
{
    // (only the base is destroyed)
}
} // namespace script
