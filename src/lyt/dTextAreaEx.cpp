#include "lyt/dTextAreaEx.h"

#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sys/dSwkbd.h"

namespace {
// the scroll of the page pane
const f32 SCROLL_SPEED = 16.0f;
const s32 SCROLL_MAX_FRAMES = 18;
const s32 SCROLL_MIN_FRAMES = 6;
const s32 SCROLL_SLOW_FRAMES = 4;
} // namespace

// 0x00501318 slot 0x3C
void lyt::TextAreaEx::ScrollToPage(s32 page)
{
    if (m_IsScrolling == true) {
        return;
    }
    m_TargetPage = page;
    nn::math::VEC3 from = m_pPagePane->m_Translate;
    nn::math::VEC3 to = from;
    from.y = m_PageTop + m_PageHeight * m_Page;
    to.y = m_PageTop + m_PageHeight * page;
    m_IsScrolling = true;
    g_Unknown95F1A4 = 1;
    m_PageMover.Stop();
    m_PageMover.Start(from, to, SCROLL_SPEED, SCROLL_MAX_FRAMES, SCROLL_MIN_FRAMES, SCROLL_SLOW_FRAMES);
    SetIndicatorVisible(false);
    vf_0x58(true, true);
    SetLineEnd(false);
}

// 0x00501414 slot 0x4C
void lyt::TextAreaEx::ScrollToTop()
{
    m_KeptPage = m_Page;
    nn::math::VEC3 from = m_pPagePane->m_Translate;
    nn::math::VEC3 to = from;
    from.y = m_PageTop + m_PageHeight * m_Page;
    to.y = 0.0f;
    m_PageMover.Stop();
    m_PageMover.Start(from, to, SCROLL_SPEED, SCROLL_MAX_FRAMES, SCROLL_MIN_FRAMES, SCROLL_SLOW_FRAMES);
    SetIndicatorVisible(false);
    vf_0x58(true, true);
}

// 0x005014E0 slot 0x44
void lyt::TextAreaEx::vf_0x44()
{
    // empty (in the original too)
}

// 0x005014E4 slot 0x48
void lyt::TextAreaEx::vf_0x48()
{
    // empty (in the original too)
}

// 0x005014E8 slot 0x40
void lyt::TextAreaEx::ResetPage()
{
    SetPagePosition(0);
    m_TopLine = 0;
}

// 0x00501510 slot 0x50
void lyt::TextAreaEx::ScrollBack()
{
    m_Page = m_KeptPage;
    const nn::math::VEC3 from = m_pPagePane->m_Translate;
    nn::math::VEC3 to = from;
    to.y = m_PageTop + m_PageHeight * m_Page;
    m_IsScrollingBack = true;
    m_PageMover.Stop();
    m_PageMover.Start(from, to, SCROLL_SPEED, SCROLL_MAX_FRAMES, SCROLL_MIN_FRAMES, SCROLL_SLOW_FRAMES);
    SetIndicatorVisible(false);
    vf_0x58(true, true);
}

// 0x005015D8 slot 0x58
void lyt::TextAreaEx::vf_0x58(bool arg1, bool arg2)
{
    // empty (in the original too)
}

// 0x005015DC slot 0x5C
bool lyt::TextAreaEx::IsIndicatorVisible()
{
    return m_pIndicatorPane->IsVisible();
}

// 0x005015F0 slot 0x54
void lyt::TextAreaEx::SetIndicatorVisible(bool visible)
{
    m_pIndicatorPane->m_Flag = (m_pIndicatorPane->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | visible;
}

// 0x00501608 slot 0x38
void lyt::TextAreaEx::ClampCursor(s32* index)
{
    // empty (in the original too)
}

// 0x0050160C slot 0x14
bool lyt::TextAreaEx::GetIndexAtPosition(s32* index, const nn::math::VEC2* pos)
{
    bool isEnd = false;
    f32 x = 0.0f;
    const nn::math::VEC3 screen(pos->x, pos->y, 0.0f);
    nn::math::VEC3 local;
    nn::math::VEC3Project(&local, &m_InvMtx, &screen);
    ScrollToCursorPage();

    // the line below the position
    f32 bottom = m_pTextBox->m_FontSize.height;
    s32 line = m_TopLine - GetLineOffset();
    for (s32 i = 0; i < GetLineNum() && -bottom > local.y; i++) {
        bottom += m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace;
        if (sys::GetLineManager()->GetLineStartChecked(line + 1) < 0) {
            break;
        }
        line++;
    }
    if (m_TopLine > line) {
        line = m_TopLine;
    } else if (m_TopLine + GetScrollLineNum() - 1 < line) {
        line = GetScrollLineNum() - 1 + m_TopLine;
    }

    // the character whose middle is right of the position
    const s32 length = sys::GetLineManager()->GetLineLength(line);
    const s32 start = sys::GetLineManager()->GetLineStartChecked(line);
    const wchar_t* text = &sys::g_pTextManager->m_pBuffer[start];
    s32 i = 0;
    for (;;) {
        if (text[i] == '\n' || text[i] == 0) {
            break;
        }
        if (i >= length) {
            isEnd = true;
            break;
        }
        const f32 width = GetCharWidth(sys::g_pTextManager->GetDisplayChar(start + i));
        x += width * 0.5f;
        if (!(local.x > x)) {
            break;
        }
        x += width * 0.5f;
        i++;
    }
    *index = start + i;
    return isEnd;
}

// 0x00501804
void lyt::TextAreaEx::ScrollToCursorPage()
{
    const s32 line = GetCursorLine();
    if (m_TopLine > line) {
        m_TopLine = line;
    } else if (m_TopLine + GetScrollLineNum() - 1 < line) {
        m_TopLine = line - GetScrollLineNum() + 1;
        ScrollToPage(m_Page + 1);
    }
    s32 page = m_Page;
    while (m_TopLine > 0 && sys::GetLineManager()->GetLineStartChecked(m_TopLine + GetScrollLineNum() - 1) < 0) {
        page--;
        m_TopLine--;
    }
    if (m_Page != page) {
        ScrollToPage(page);
    }
}

// 0x00501918 slot 0x60
void lyt::TextAreaEx::SetPagePosition(s32 page)
{
    nn::math::VEC3 pos = m_pPagePane->m_Translate;
    pos.y = m_PageTop + m_PageHeight * page;
    m_pPagePane->SetTranslate(pos);
}

// 0x00501968
lyt::TextAreaEx::TextAreaEx(u32 unknown, s32 priority, sead::Heap* heap)
    : TextArea(unknown, priority, heap), m_pPagePane(NULL), m_pIndicatorPane(NULL), m_PageHeight(0.0f), m_PageTop(0.0f), m_Page(0),
      m_TargetPage(0), m_Unknown0x730(0), m_KeptPage(0), m_IsScrolling(false), m_IsScrollingBack(false), m_Unknown0x73A(false),
      m_ScrollMaxFrames(SCROLL_MAX_FRAMES), m_ScrollMinFrames(SCROLL_MIN_FRAMES), m_ScrollSlowFrames(SCROLL_SLOW_FRAMES), m_ScrollSpeed(SCROLL_SPEED)
{
}

// 0x0050C44C slot 0x00 (falls into ~TextArea)
// 0x005019EC (deleting dtor)
lyt::TextAreaEx::~TextAreaEx()
{
    // (only the base is destroyed)
}
