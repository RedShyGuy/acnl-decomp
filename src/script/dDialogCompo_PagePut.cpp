#include "script/dDialogCompo_PagePut.h"
#include "script/dRenderTag.h"

// 0x005D98EC slot 0x14 (name is ours)
void script::DialogCompo::PagePut::OnChar(const CharInfo* info)
{
    m_IsSuccess = PutChar(&m_pWrite, &m_pLast, info) & m_IsSuccess;
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005D9924 slot 0x18 (name is ours)
void script::DialogCompo::PagePut::OnTag(const Tag* tag)
{
    s32 group = tag->group;
    if ((group == 0 && tag->index == 4) || group == 19) {
        // the page breaks and the values of the dialog are not written
        return;
    }
    m_IsSuccess = PutTag(&m_pWrite, &m_pLast, tag) & m_IsSuccess;
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005D9988 slot 0x00 (name is ours)
// 0x005D9978 (deleting dtor)
script::DialogCompo::PagePut::~PagePut()
{
    // empty (in the original too)
}
