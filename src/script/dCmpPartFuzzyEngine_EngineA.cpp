#include "script/dCmpPartFuzzyEngine_EngineA.h"

namespace script {
// 0x005E96F4 slot 0x14
void CmpPartFuzzyEngine::EngineA::OnChar(const CharInfo* info)
{
    m_B.m_pInfo = info;
    m_B.m_IsContinue = true;
    m_B.m_IsMatch = true;
    m_B.SetText2(&s_pNoStop);
    m_B.m_pInfo = NULL;
    if (m_B.m_IsEnd) {
        m_B.m_IsEqual = false;
    }
    if (!m_B.m_IsMatch) {
        m_B.m_IsFound = false;
        m_B.m_IsEqual = false;
    }
}

// 0x005E9748 slot 0x18
void CmpPartFuzzyEngine::EngineA::OnTag(const Tag* tag)
{
    m_B.m_pTag = tag;
    m_B.m_IsContinue = true;
    m_B.m_IsMatch = true;
    m_B.SetText2(&s_pNoStop);
    m_B.m_pTag = NULL;
    if (m_B.m_IsEnd) {
        m_B.m_IsEqual = false;
    }
    if (!m_B.m_IsMatch) {
        m_B.m_IsFound = false;
        m_B.m_IsEqual = false;
    }
}

// 0x005E979C slot 0x10
bool CmpPartFuzzyEngine::EngineA::IsContinue()
{
    return m_B.m_IsEqual;
}

// 0x005E97B8 slot 0x00
// 0x005E97A4 (deleting dtor)
CmpPartFuzzyEngine::EngineA::~EngineA()
{
    // (only the members and the base are destroyed)
}
} // namespace script
