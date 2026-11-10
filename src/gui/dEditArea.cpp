#include "gui/dEditArea.h"

#include "Other/dController.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "lyt/dTextArea.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Group.h"
#include "sys/dLineManager.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"

namespace gui {
namespace {
// the keyboards whose text field takes no touch (see sys::g_Unknown95F0EB)
const u8 KEYBOARD_NO_TOUCH_1 = 17;
const u8 KEYBOARD_NO_TOUCH_2 = 18;
// a scroll needs this horizontal move of the touch before its interval is over
const f32 SCROLL_MOVE_X = 25.0f;
} // namespace

// 0x004FFB18 slot 0x28
void EditArea::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    const u8 keyboard = sys::g_Unknown95F0EB;
    if (keyboard == KEYBOARD_NO_TOUCH_1 || keyboard == KEYBOARD_NO_TOUCH_2) {
        return;
    }
    if (!Controller::IsTrig(Controller::TOUCH) || !sys::IsTouchInPane(m_pBounding)) {
        return;
    }
    sys::TextManager* textManager = sys::g_pTextManager;
    textManager->EndCellPhone(true);
    textManager->Commit(false);
    const s32 cursor = textManager->m_Cursor;
    s32 index;
    const bool isLineEnd = m_pTextArea->GetIndexAtPosition(&index, &g_TouchPosition);
    // the cursor moves (or something is being composed or selected)
    if (index != cursor || textManager->m_IsSelecting ||
        textManager->m_Length > textManager->m_Length - textManager->m_CompositionLength ||
        (textManager->m_pCellPhoneInput != NULL && textManager->m_Cursor > 0 && textManager->m_Cursor - 1 >= 0) ||
        m_pTextArea->UpdateLineEnd() != isLineEnd) {
        SoundMgr::SPlaySystemSound(SE_SYS_SWK_CURSOR, sys::CalcPaneSoundPan(m_pTextArea->m_pCursorPane));
    }
    textManager->SetCursor(index, true, true);
    m_pTextArea->SetLineEnd(isLineEnd);
    m_IsFirstScroll = true;
    m_ScrollFrames = 0;
    m_OutsideFrames = 0;
    m_LastScrollTouch = g_TouchPosition;
    const f32 y = m_LastScrollTouch.y;
    m_DragTop = m_AreaTop;
    m_DragBottom = m_AreaBottom;
    if (y > m_AreaTop) {
        m_DragTop = y;
    }
    if (y < m_AreaBottom) {
        m_DragBottom = y;
    }
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    m_IsHolding = true;
    g_IsWidgetHolding = true;
    m_State = STATE_PRESSED;
    SendEvent(this, EVENT_DECIDE, 0);
}

// 0x004FFCD4 slot 0x2C
void EditArea::UpdatePressed()
{
    sys::TextManager* textManager = sys::g_pTextManager;
    if (!Controller::IsHold(Controller::TOUCH)) {
        // released: an empty selection ends
        const s32 cursor = textManager->m_Cursor;
        const s32 anchor = textManager->m_SelectionAnchor;
        const s32 end = anchor > cursor ? anchor : cursor;
        const s32 start = anchor > cursor ? cursor : anchor;
        if (end == start) {
            textManager->m_IsSelecting = false;
            textManager->m_SelectionAnchor = cursor;
            textManager->m_Unknown0x90 = -1;
        }
        m_State = STATE_DECIDED;
        return;
    }
    const f32 x = g_TouchPosition.x;
    const f32 y = g_TouchPosition.y;
    bool isScrolled = false;
    if (y < m_DragTop && y > m_DragBottom) {
        // inside: no scrolling
        m_ScrollFrames = 0;
        m_OutsideFrames = 0;
    } else {
        m_ScrollFrames++;
        const f32 distance = y >= m_DragTop ? y - m_DragTop : y - m_DragBottom;
        // the frames between two scrolls
        s32 interval;
        if (m_IsFirstScroll) {
            interval = 30;
        } else if (distance >= 0.0f) {
            if (distance <= 6.0f) {
                interval = 20;
                m_OutsideFrames = 0;
            } else {
                m_OutsideFrames++;
                if (m_OutsideFrames <= 60) {
                    interval = 20;
                } else if (m_OutsideFrames <= 120) {
                    interval = 10;
                } else if (m_OutsideFrames <= 180) {
                    interval = 5;
                } else {
                    m_OutsideFrames--;
                    interval = 2;
                }
            }
        } else if (distance >= -8.0f) {
            interval = 20;
        } else if (distance >= -50.0f) {
            interval = 10;
        } else if (distance >= -100.0f) {
            interval = 5;
        } else {
            interval = 2;
        }
        bool isScroll = true;
        if (m_ScrollFrames < interval) {
            // (only the first one waits for its interval; then a move to the side scrolls at once)
            f32 moveX = x - m_LastScrollTouch.x;
            if (moveX < 0.0f) {
                moveX = -moveX;
            }
            if (m_IsFirstScroll || moveX < SCROLL_MOVE_X) {
                isScroll = false;
            }
        }
        if (isScroll) {
            m_LastScrollTouch.x = x;
            m_LastScrollTouch.y = y;
            m_IsFirstScroll = false;
            m_ScrollFrames = 0;
            if (y >= m_DragTop) {
                if (m_pTextArea->m_TopLine > 0) {
                    m_pTextArea->m_TopLine--;
                    PlaySound(SOUND_SLIDE_OUT, 0.0f);
                    isScrolled = true;
                }
            } else {
                sys::LineManager* lineManager = textManager != NULL ? textManager->m_pLineManager : NULL;
                if (lineManager->GetLineStartChecked(m_pTextArea->m_TopLine + m_pTextArea->GetScrollLineNum()) >= 0) {
                    m_pTextArea->m_TopLine++;
                    PlaySound(SOUND_SLIDE_OUT, 0.0f);
                    isScrolled = true;
                }
            }
        }
    }
    const s32 cursor = textManager->m_Cursor;
    s32 index;
    const bool isLineEnd = m_pTextArea->GetIndexAtPosition(&index, &g_TouchPosition);
    if (!isScrolled && index != cursor) {
        SoundMgr::SPlaySystemSound(SE_SYS_SWK_CURSOR, sys::CalcPaneSoundPan(m_pTextArea->m_pCursorPane));
        SoundMgr::SPlaySystemSound(SE_SYS_SWK_CURSOR_DRAG, sys::CalcPaneSoundPan(m_pTextArea->m_pCursorPane));
    }
    textManager->SetCursor(index, true, false);
    m_pTextArea->SetLineEnd(isLineEnd);
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    SendEvent(this, EVENT_DECIDE, 1);
}

// 0x00500054
EditArea::EditArea(const Description& desc, lyt::TextArea* textArea)
    : TchOnButton(desc), m_pTextArea(textArea), m_DragTop(0.0f), m_DragBottom(0.0f), m_ScrollFrames(0), m_OutsideFrames(0),
      m_LastScrollTouch(0.0f, 0.0f), m_IsFirstScroll(true)
{
    nw::lyt::Group* group = textArea->m_pTextAreaGroup;
    if (group == NULL) {
        group = textArea->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(m_GroupName);
    }
    m_pBounding = nw::lyt::DynamicCast<nw::lyt::Bounding>(
        nw::lyt::Group::PaneLinkList::GetPointerFromNode(group->GetPaneList().begin().GetNode())->m_pTarget);
    const nw::ut::Rect rect = m_pBounding->GetPaneRect();
    const f32 top = rect.top + m_pBounding->m_GlbMtx.m[1][3];
    const f32 bottom = top - m_pBounding->m_Size.height;
    m_AreaTop = top - SCROLL_MARGIN;
    m_AreaBottom = bottom + SCROLL_MARGIN;
    sys::g_pTextManager->m_pSoundPane = textArea->m_pCursorPane;
}

// 0x00500198 slot 0x00
// 0x00500168 (deleting dtor)
EditArea::~EditArea()
{
    // (only the base is destroyed)
}
} // namespace gui
