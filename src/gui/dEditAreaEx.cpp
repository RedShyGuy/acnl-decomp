#include "gui/dEditAreaEx.h"

#include "Other/dController.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "lyt/dTextAreaEx.h"
#include "nw/lyt/lyt_Bounding.h"
#include "sys/dLineManager.h"
#include "sys/dTextManager.h"

namespace gui {
namespace {
// a scroll needs this horizontal move of the touch before its interval is over
const f32 SCROLL_MOVE_X = 25.0f;
} // namespace

// 0x004F3B24 slot 0x28
void EditAreaEx::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
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
    SendEvent(this, EVENT_PRESS, 0);
}

// 0x004F3CD0 slot 0x2C
void EditAreaEx::UpdatePressed()
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
        SendEvent(this, EVENT_RELEASE, 0);
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
            interval = 15;
        } else if (distance >= 0.0f) {
            if (distance <= 6.0f) {
                interval = 5;
                m_OutsideFrames = 0;
            } else {
                m_OutsideFrames++;
                if (m_OutsideFrames <= 30) {
                    interval = 5;
                } else if (m_OutsideFrames <= 60) {
                    interval = 3;
                } else if (m_OutsideFrames <= 90) {
                    interval = 2;
                } else {
                    m_OutsideFrames--;
                    interval = 1;
                }
            }
        } else if (distance >= -8.0f) {
            interval = 5;
        } else if (distance >= -50.0f) {
            interval = 3;
        } else if (distance >= -100.0f) {
            interval = 2;
        } else {
            interval = 1;
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
                    GetTextAreaEx()->vf_0x44();
                }
            } else {
                sys::LineManager* lineManager = textManager != NULL ? textManager->m_pLineManager : NULL;
                if (lineManager->GetLineStartChecked(m_pTextArea->m_TopLine + m_pTextArea->GetScrollLineNum()) >= 0) {
                    m_pTextArea->m_TopLine++;
                    PlaySound(SOUND_SLIDE_OUT, 0.0f);
                    isScrolled = true;
                    GetTextAreaEx()->vf_0x48();
                }
            }
        }
    }
    const s32 cursor = textManager->m_Cursor;
    s32 index;
    const bool isLineEnd = m_pTextArea->GetIndexAtPosition(&index, &g_TouchPosition);
    GetTextAreaEx()->ClampCursor(&index);
    if (!isScrolled && index != cursor) {
        SoundMgr::SPlaySystemSound(SE_SYS_SWK_CURSOR, sys::CalcPaneSoundPan(m_pTextArea->m_pCursorPane));
        SoundMgr::SPlaySystemSound(SE_SYS_SWK_CURSOR_DRAG, sys::CalcPaneSoundPan(m_pTextArea->m_pCursorPane));
    }
    textManager->SetCursor(index, true, false);
    m_pTextArea->SetLineEnd(isLineEnd);
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    SendEvent(this, EVENT_DECIDE, 2);
}

// 0x004F40C0 slot 0x00
// 0x004F4090 (deleting dtor)
EditAreaEx::~EditAreaEx()
{
    // (only the base is destroyed)
}
} // namespace gui
