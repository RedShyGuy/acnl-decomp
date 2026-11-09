#include "script/dCmpFuzzyEngine.h"

namespace script {
// 0x005E5790 slot 0x14
void CmpFuzzyEngine::OnChar(const CharInfo* info)
{
    m_B.m_IsContinue = true;
    m_B.m_IsMatch = false;
    m_B.m_pInfo = info;
    m_B.SetText2(&s_pNoStop);
    m_B.m_pInfo = NULL;
    if (!m_B.m_IsMatch) {
        m_B.m_IsEqual = false;
    }
}

// 0x005E57D4 slot 0x18
void CmpFuzzyEngine::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x005E5918 slot 0x10
bool CmpFuzzyEngine::IsContinue()
{
    return m_B.m_IsEqual;
}

// 0x005E5934 slot 0x00
// 0x005E5920 (deleting dtor)
CmpFuzzyEngine::~CmpFuzzyEngine()
{
    // (only the members and the base are destroyed)
}
} // namespace script
