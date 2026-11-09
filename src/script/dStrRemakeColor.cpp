#include "script/dStrRemakeColor.h"

namespace script {
// 0x005E6E7C slot 0x14
void StrRemakeColor::OnChar(const CharInfo* info)
{
    if (info->units == '\n') {
        m_CurLine++;
        if (m_CurLine > m_Line) {
            m_IsStop = 1;
        }
        return;
    }
    if (m_CurLine == m_Line && !PutChar(&m_pDst, &m_pDstLast, info)) {
        m_IsFull = 1;
    }
}

// 0x005E6ED4 slot 0x0C
void StrRemakeColor::End()
{
    if (m_CurLine < m_Line) {
        m_IsFull = 1;
    }
}

// 0x005E6EEC slot 0x18
void StrRemakeColor::OnTag(const Tag* tag)
{
    if (m_CurLine == m_Line && !PutTag(&m_pDst, &m_pDstLast, tag)) {
        m_IsFull = 1;
    }
}

// 0x005E6F24 (name is ours)
bool StrRemakeColor::CopyLine(IWord* dst, const IWord* src, s32 line)
{
    m_IsFull = 0;
    if (dst != NULL) {
        dst->Clear();
        m_pDst = dst->GetWord();
        m_pDstLast = m_pDst;
        m_pDstLast = m_pDstLast + dst->GetSize() - 1;
        m_IsStop = 0;
        m_Line = line;
        m_CurLine = 0;
        Run(src->GetWord());
    }
    return m_IsFull == 0;
}

// 0x005E6FE0 slot 0x10
bool StrRemakeColor::IsContinue()
{
    return m_IsStop == 0;
}

// 0x005E7004 slot 0x00
// 0x005E6FF4 (deleting dtor)
StrRemakeColor::~StrRemakeColor()
{
    // (only the base is destroyed)
}
} // namespace script
