#pragma once

#include "decomp.h"
#include "lyt/dTextArea.h"
#include "ssys/ma/ma_Math.h"

namespace lyt {
// A TextArea of three lines whose text pane scrolls page by page (with a Mover) when the cursor leaves the page
// (letters and the bulletin board derive from it). Only the class name is from the RTTI; all other names are ours.
// RTTI N3lyt10TextAreaExE @ 0x008D0F28
// vtable 0x00904248 (vptr 0x00904250), offset_to_top 0, 25 entries
class TextAreaEx : public ::lyt::TextArea
{
public:
    TextAreaEx(u32 unknown, s32 priority, sead::Heap* heap); // 0x00501968
    virtual ~TextAreaEx(); // 0x0050C44C slot 0x00 (falls into ~TextArea)
    // 0x005019EC slot 0x04 (deleting dtor)
    bool GetIndexAtPosition(s32* index, const nn::math::VEC2* pos) override; // 0x0050160C slot 0x14
    s32 GetDecorNum() override { return 3; } // 0x0074789C slot 0x20
    s32 GetLineNum() override { return 3; } // 0x00747884 slot 0x24
    s32 GetScrollLineNum() override { return 2; } // 0x0074788C slot 0x28
    s32 GetLineOffset() override { return 1; } // 0x00747894 slot 0x2C
    // the position for the cursor is corrected (the letter keeps it off the name in the header)
    virtual void ClampCursor(s32* index); // 0x00501608 slot 0x38
    // the page pane moves to the page
    virtual void ScrollToPage(s32 page); // 0x00501318 slot 0x3C
    // the first page at once
    virtual void ResetPage(); // 0x005014E8 slot 0x40
    virtual void vf_0x44(); // 0x005014E0 slot 0x44
    virtual void vf_0x48(); // 0x005014E4 slot 0x48
    // the page pane moves to the top (the page is kept for ScrollBack)
    virtual void ScrollToTop(); // 0x00501414 slot 0x4C
    // the page pane moves back to the kept page
    virtual void ScrollBack(); // 0x00501510 slot 0x50
    virtual void SetIndicatorVisible(bool visible); // 0x005015F0 slot 0x54
    // (called when a scroll starts)
    virtual void vf_0x58(bool arg1, bool arg2); // 0x005015D8 slot 0x58
    virtual bool IsIndicatorVisible(); // 0x005015DC slot 0x5C
    // the page pane is put at the page
    virtual void SetPagePosition(s32 page); // 0x00501918 slot 0x60

    // the first line follows the cursor and the page follows the first line
    void ScrollToCursorPage(); // 0x00501804

    ssys::ma::Mover m_PageMover;      // 0x6F8
    nw::lyt::Pane* m_pPagePane;       // 0x718
    nw::lyt::Pane* m_pIndicatorPane;  // 0x71C
    f32 m_PageHeight;                 // 0x720 y per page
    f32 m_PageTop;                    // 0x724 y of page 0
    s32 m_Page;                       // 0x728
    s32 m_TargetPage;                 // 0x72C
    s32 m_Unknown0x730;               // 0x730
    s32 m_KeptPage;                   // 0x734
    bool m_IsScrolling;               // 0x738
    bool m_IsScrollingBack;           // 0x739
    bool m_Unknown0x73A;              // 0x73A
    s32 m_ScrollMaxFrames;            // 0x73C
    s32 m_ScrollMinFrames;            // 0x740
    s32 m_ScrollSlowFrames;           // 0x744
    f32 m_ScrollSpeed;                // 0x748
};
ASSERT_OFFSET(TextAreaEx, m_pPagePane, 0x718);
ASSERT_OFFSET(TextAreaEx, m_ScrollSpeed, 0x748);
ASSERT_SIZE(TextAreaEx, 0x74C);
} // namespace lyt
