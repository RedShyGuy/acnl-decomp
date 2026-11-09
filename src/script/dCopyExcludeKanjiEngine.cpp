#include "script/dCopyExcludeKanjiEngine.h"
#include <cstring>

namespace script {
// 0x005EA0D8 slot 0x14
void CopyExcludeKanjiEngine::OnChar(const CharInfo* info)
{
    if (!PutChar(&m_pDst, &m_pDstLast, info)) {
        m_IsFull = 1;
    }
}

// 0x005EA100 slot 0x18
void CopyExcludeKanjiEngine::OnTag(const Tag* tag)
{
    if (tag->group == 0) {
        OnRuby(tag);
        return;
    }
    if (!PutTag(&m_pDst, &m_pDstLast, tag)) {
        m_IsFull = 1;
    }
}

// 0x005EA140 (name is ours)
void CopyExcludeKanjiEngine::OnRuby(const Tag* tag)
{
    // the parameters: the size of the base text and of the reading in bytes, then the reading; the base text follows
    const u8* tagEnd = reinterpret_cast<const u8*>(tag->data) + (tag->length & ~1);
    const char16* args = tag->args;
    const char16* next = reinterpret_cast<const char16*>(tagEnd + (args[0] & ~1));
    const char16* reading = args + 2;
    const char16* readingEnd = reinterpret_cast<const char16*>(reinterpret_cast<const u8*>(reading) + (args[1] & ~1));
    s32 num = readingEnd - reading;
    if (num > 0) {
        char16* end = m_pDst + num;
        if (end > m_pDstLast) {
            s32 fit = num - (end - m_pDstLast);
            if (fit > 0) {
                std::memcpy(m_pDst, reading, fit * sizeof(char16));
                m_pDst += fit;
            }
            m_IsFull = 1;
        } else {
            std::memcpy(m_pDst, reading, num * sizeof(char16));
            m_pDst = end;
        }
    }
    m_pCur = next;
}

// 0x005EA1E4 (name is ours)
void CopyExcludeKanjiEngine::Copy(IWord* dst, const IWord* src)
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
    dst->Clear();
    m_pDstLast = buffer + size - 1;
    m_pDst = buffer;
    m_IsFull = 0;
    Run(text);
}

// 0x005EA2B4 slot 0x10
bool CopyExcludeKanjiEngine::IsContinue()
{
    return m_IsFull == 0;
}

// 0x005EA2C8 (name after the RTTI)
CopyExcludeKanjiEngine::CopyExcludeKanjiEngine() : m_pDst(NULL), m_pDstLast(NULL), m_IsFull(0)
{
}

// 0x005EA300 slot 0x00
// 0x005EA2F0 (deleting dtor)
CopyExcludeKanjiEngine::~CopyExcludeKanjiEngine()
{
    // (only the base is destroyed)
}
} // namespace script
