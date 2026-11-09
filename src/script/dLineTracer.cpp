#include "script/dLineTracer.h"
#include <cstring>

namespace script {
// 0x005D61B4 slot 0x14
void LineTracer::OnChar(const CharInfo* info)
{
    if (info->units == '\n') {
        m_IsReturn = true;
        m_pLine = m_pCur;
        return;
    }
    PutChar(&m_pDst, &m_pDstLast, info);
}

// 0x005D61E4 (name is ours)
void LineTracer::SetSource(const IWord* word)
{
    m_pLine = word->GetWord();
}

// 0x005D6204 slot 0x0C
void LineTracer::End()
{
    // empty (in the original too)
}

// 0x005D6208 slot 0x18
void LineTracer::OnTag(const Tag* tag)
{
    PutTag(&m_pDst, &m_pDstLast, tag);
}

// 0x005D6244 slot 0x10
bool LineTracer::IsContinue()
{
    return !m_IsReturn;
}

// 0x005D6258 (name is ours)
bool LineTracer::ReadLine(IWord* dst)
{
    dst->Clear();
    m_pDst = dst->GetWord();
    m_pDstLast = m_pDst;
    m_pDstLast = m_pDstLast + dst->GetSize() - 1;
    m_IsReturn = false;
    SetText(&m_pLine, &s_pNoStop);
    SetText2(&s_pNoStop);
    return m_IsReturn;
}

// 0x005D62E0 (name after the RTTI)
LineTracer::LineTracer() : m_pLine(NULL), m_pDst(NULL), m_pDstLast(NULL), m_IsReturn(false)
{
}

// 0x005D631C slot 0x00
// 0x005D630C (deleting dtor)
LineTracer::~LineTracer()
{
    // (only the base is destroyed)
}
} // namespace script
