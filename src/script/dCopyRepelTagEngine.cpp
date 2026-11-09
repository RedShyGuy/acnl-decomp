#include "script/dCopyRepelTagEngine.h"

namespace script {
// 0x005E9B40 slot 0x14
void CopyRepelTagEngine::OnChar(const CharInfo* info)
{
    if (!PutChar(&m_pDst, &m_pDstLast, info)) {
        m_IsFull = 1;
    }
}

// 0x005E9B68 slot 0x10
bool CopyRepelTagEngine::IsContinue()
{
    return m_IsFull == 0;
}

// 0x005E9B8C slot 0x00
// 0x005E9B7C (deleting dtor)
CopyRepelTagEngine::~CopyRepelTagEngine()
{
    // (only the base is destroyed)
}
} // namespace script
