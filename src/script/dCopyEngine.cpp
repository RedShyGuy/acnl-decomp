#include "script/dCopyEngine.h"
#include "script/dWordPtr.h"
#include "nn/nstd/nstd_String.h"
#include <cstring>

namespace script {
// 0x005D5A38 (name is ours)
void CopyEngine::Copy(IWord* dst, const IWord* src)
{
    const char16* text = src->GetWord();
    u32 srcSize = src->GetSize();
    if (dst == NULL || text == NULL || srcSize == 0 || dst == src) {
        return;
    }
    char16* buffer = dst->GetWord();
    u32 size = dst->GetSize();
    if (buffer == NULL || size == 0) {
        return;
    }
    m_pLimit = (size < srcSize ? text + size : text + srcSize) - 1;
    m_pStop = text;
    buffer[0] = WORD_NULL_CHAR;
    if (m_pLimit <= text) {
        return;
    }
    Run(text);
    if (m_pStop <= text) {
        return;
    }
    u32 length = m_pStop - text;
    if (size <= length) {
        return;
    }
    nnnstdMemCpy(buffer, text, length * sizeof(char16));
    std::memset(buffer + length, 0, (size - length) * sizeof(char16));
}

// 0x005D5B58 slot 0x10
bool CopyEngine::IsContinue()
{
    bool isContinue = m_pLimit > m_pCur;
    if (m_pLimit >= m_pCur) {
        m_pStop = m_pCur;
    }
    return isContinue;
}

// 0x005D5B88 slot 0x00
// 0x005D5B78 (deleting dtor)
CopyEngine::~CopyEngine()
{
    // (only the base is destroyed)
}
} // namespace script
