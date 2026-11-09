#pragma once

#include "decomp.h"
#include "script/dDialogCompo.h"
#include "script/dMsgEngine.h"

// Finds the pages of the text: the starts and ends of the pages, the values of the tags 19/x (names are ours).
// RTTI N6script11DialogCompo11PageAnalyzeE @ 0x008D3798
// vtable 0x00909DC4 (vptr 0x00909DCC), offset_to_top 0, 8 entries
class script::DialogCompo::PageAnalyze : public ::script::MsgEngine
{
public:
    // (inline)
    PageAnalyze(s32 lines, Info* pInfo, const char16** pStarts, const char16** pEnds)
        : m_Lines(lines), m_pInfo(pInfo), m_pStarts(pStarts), m_pEnds(pEnds), m_MaxPages(PAGE_NUM), m_LineCount(0),
          m_PageNum(0)
    {
    }
    virtual ~PageAnalyze(); // 0x005D94FC slot 0x00
    // 0x005D94EC slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x005D930C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005D93A0 slot 0x18

    // (inline) a new page starts at the current position
    void StartPage()
    {
        m_LineCount = 0;
        if (m_PageNum < m_MaxPages) {
            m_pStarts[m_PageNum] = m_pCur;
            m_PageNum++;
        }
    }
    // (inline) the current page ends at the current position
    void UpdatePageEnd()
    {
        s32 page = m_PageNum - 1;
        if (page >= 0 && page < m_MaxPages) {
            m_pEnds[page] = m_pCur;
        }
    }

    s32 m_Lines;               // 0x60 the lines of a page
    Info* m_pInfo;             // 0x64
    const char16** m_pStarts;  // 0x68
    const char16** m_pEnds;    // 0x6C
    s32 m_MaxPages;            // 0x70
    s32 m_LineCount;           // 0x74
    s32 m_PageNum;             // 0x78
};
