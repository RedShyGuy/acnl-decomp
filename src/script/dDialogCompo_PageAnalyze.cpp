#include "script/dDialogCompo_PageAnalyze.h"
#include "script/dRenderTag.h"

// 0x005D930C slot 0x14 (name is ours)
void script::DialogCompo::PageAnalyze::OnChar(const CharInfo* info)
{
    if (info->units == '\n') {
        m_LineCount++;
        if (m_LineCount >= m_Lines) {
            StartPage();
            return;
        }
    }
    UpdatePageEnd();
}

// 0x005D93A0 slot 0x18 (name is ours)
void script::DialogCompo::PageAnalyze::OnTag(const Tag* tag)
{
    s32 group = tag->group;
    s32 index = tag->index;
    if (group == 0 && index == 4) {
        // the page break
        StartPage();
        return;
    }
    UpdatePageEnd();
    if (group != 19) {
        return;
    }
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    switch (tag->index) {
    case 0:
        m_pInfo->kind = 0;
        break;
    case 1:
        m_pInfo->kind = 1;
        m_pInfo->values[0] = *reinterpret_cast<const u16*>(args);
        m_pInfo->flag = args[2] == 1 ? 0 : -1;
        break;
    case 2:
        m_pInfo->kind = 2;
        m_pInfo->values[0] = reinterpret_cast<const u16*>(args)[0];
        m_pInfo->values[1] = reinterpret_cast<const u16*>(args)[1];
        if (args[4] == 1) {
            m_pInfo->flag = 0;
        } else if (args[4] == 2) {
            m_pInfo->flag = 1;
        } else {
            m_pInfo->flag = -1;
        }
        break;
    case 3:
        if (args[0] < 5) {
            m_pInfo->value = args[0];
        }
        break;
    }
}

// 0x005D94FC slot 0x00 (name is ours)
// 0x005D94EC (deleting dtor)
script::DialogCompo::PageAnalyze::~PageAnalyze()
{
    // empty (in the original too)
}
