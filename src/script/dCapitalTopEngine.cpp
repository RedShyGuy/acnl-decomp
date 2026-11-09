#include "script/dCapitalTopEngine.h"
#include "script/dToUpper.h"

namespace script {
// 0x005E7138 slot 0x14
void CapitalTopEngine::OnChar(const CharInfo*)
{
    if (m_pChar != NULL && ToUpper(m_pChar)) {
        m_IsModified = true;
    }
    m_IsContinue = false;
}

// 0x005E7168 slot 0x10
bool CapitalTopEngine::IsContinue()
{
    return m_IsContinue;
}

// 0x005E7174 slot 0x00
// 0x005E7170 (deleting dtor)
CapitalTopEngine::~CapitalTopEngine()
{
    // empty (in the original too)
}
} // namespace script
