#include "script/dSearcher.h"

namespace script {
// 0x005FDC50 (name is ours)
void Searcher::OnTagFind(const Tag* tag)
{
    if (m_Group == tag->group && m_Index == tag->index) {
        m_pFound = tag->data;
        m_IsFound = true;
    }
}

// 0x005FDC80 slot 0x14
void Searcher::OnChar(const CharInfo* info)
{
    CharFunc func = s_CharFuncs[m_Mode];
    if (func != NULL) {
        (this->*func)(info);
    }
}

// 0x005FDCCC (name is ours)
void Searcher::OnCharFind(const CharInfo* info)
{
    if (m_Char == info->units) {
        m_pFound = info->pText;
        m_IsFound = true;
    }
}

// 0x005FDCF0 (name is ours)
void Searcher::OnTagGroup(const Tag* tag)
{
    s32 group = tag->group;
    if (group == 1 || group == 4 || group == 5 || group == 17) {
        m_pFound = tag->data;
        m_IsFound = true;
    }
}

// 0x005FDD20 (name is ours)
void Searcher::OnCharNotReturn(const CharInfo* info)
{
    if (info->units != '\n') {
        m_IsFound = true;
    }
}

// 0x005FDD34 (name is ours)
void Searcher::OnCharReturn(const CharInfo* info)
{
    if (info->units == '\n') {
        m_pFound = info->pText;
        m_IsFound = true;
    }
}

// 0x005FDD54 slot 0x0C
void Searcher::End()
{
    if (m_Mode == MODE_RETURN && !m_IsFound) {
        // the end of the text counts as a new line
        m_IsFound = true;
        m_pFound = m_pCur;
    }
}

// 0x005FDD7C slot 0x18
void Searcher::OnTag(const Tag* tag)
{
    TagFunc func = s_TagFuncs[m_Mode];
    if (func != NULL) {
        (this->*func)(tag);
    }
}

// 0x005FDDC8 slot 0x08
void Searcher::Begin()
{
    // empty (in the original too)
}

// 0x005FDDCC slot 0x10
bool Searcher::IsContinue()
{
    return !m_IsFound;
}

// 0x005FDDE0 (name after the RTTI)
Searcher::Searcher()
    : m_Mode(MODE_NONE), m_Unknown0x64(0), m_Char(0), m_Group(0x7FFFFFFF), m_Index(-1), m_IsFound(false), m_pFound(NULL)
{
}

// 0x005FDE34 slot 0x00
// 0x005FDE24 (deleting dtor)
Searcher::~Searcher()
{
    // (only the base is destroyed)
}
} // namespace script
