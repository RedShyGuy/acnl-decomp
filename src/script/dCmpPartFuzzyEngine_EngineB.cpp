#include "script/dCmpPartFuzzyEngine_EngineB.h"
#include <cstring>

namespace script {
// 0x005E97CC slot 0x14
void CmpPartFuzzyEngine::EngineB::OnChar(const CharInfo* info)
{
    if (m_pInfo == NULL || !IsFuzzyEqual(*m_pInfo->pText, *info->pText)) {
        m_IsMatch = false;
    }
    m_IsContinue = false;
}

// 0x005E98CC slot 0x0C
void CmpPartFuzzyEngine::EngineB::End()
{
    m_IsEnd = true;
}

// 0x005E98D8 slot 0x18
void CmpPartFuzzyEngine::EngineB::OnTag(const Tag* tag)
{
    if (m_pTag == NULL || m_pTag->length != tag->length || std::memcmp(m_pTag->data, tag->data, m_pTag->length) != 0) {
        m_IsMatch = false;
    }
    m_IsContinue = false;
}

// 0x005E9920 slot 0x10
bool CmpPartFuzzyEngine::EngineB::IsContinue()
{
    return m_IsContinue;
}

// 0x005E9938 slot 0x00
// 0x005E9928 (deleting dtor)
CmpPartFuzzyEngine::EngineB::~EngineB()
{
    // (only the base is destroyed)
}
} // namespace script
