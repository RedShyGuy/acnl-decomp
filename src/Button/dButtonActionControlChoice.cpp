#include "Button/dButtonActionControlChoice.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dTouchState.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Bounding.h"

namespace {
const u32 INPUT_OK = 4;
} // namespace

// 0x0033F100 slot 0x1C (name after libgarden)
void ButtonActionControlChoice::CheckInputForNodeOk()
{
    if (m_pCurrent == NULL) {
        // the pressed button becomes the current one
        for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
            if (node->m_IsPressed == true) {
                m_pCurrent = node;
                break;
            }
        }
    }
    if (IsNextPressed()) {
        if (m_pCurrent == NULL) {
            return;
        }
        ButtonActionNode* next = static_cast<ButtonActionNode*>(m_pCurrent->m_pNext);
        if (next == NULL) {
            return;
        }
        if (!next->IsActivated()) {
            next = GetNextActive(next);
        }
        if (next == NULL) {
            return;
        }
        m_CurrentIndex++;
        m_pCurrent->Select(false, false);
        m_pCurrent = next;
        next->Select(true, true);
        m_pCurrent->MoveHandCursor();
    } else if (IsPrevPressed()) {
        if (m_pCurrent == NULL) {
            return;
        }
        ButtonActionNode* prev = static_cast<ButtonActionNode*>(m_pCurrent->m_pPrev);
        if (prev == NULL) {
            return;
        }
        if (!prev->IsActivated()) {
            prev = GetPrevActive(prev);
        }
        if (prev == NULL) {
            return;
        }
        m_CurrentIndex--;
        m_pCurrent->Select(false, false);
        m_pCurrent = prev;
        prev->Select(true, true);
        m_pCurrent->MoveHandCursor();
    } else {
        BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
        bool ok = (m_Flags2 & WAIT_FOR_SELECT_RELEASE) != 0 ? mgr->IsRelease(INPUT_OK) : mgr->IsPressed(INPUT_OK);
        if (ok) {
            if (m_pCurrent != NULL) {
                m_pCurrent->Animate(m_pCurrent->m_Touch, true);
                SelectCurrent();
            }
        } else if (m_pCheckInput != NULL) {
            m_pCheckInput();
        }
    }
}

// 0x0033F3A4 slot 0x10 (name after libgarden)
void ButtonActionControlChoice::Process()
{
    if ((m_Flags2 & ENABLE_TOUCH) == 0) {
        return;
    }
    TouchState* touch = &g_TouchState;
    if (!touch->isTouch) {
        if (m_pCurrent == NULL) {
            return;
        }
        m_IsTouchOnButton = 0;
        if (m_pCurrent->IsTouch()) {
            SelectCurrent();
            return;
        }
        m_pCurrent = NULL;
        m_PlayTouchSound = false;
        if (m_EnableCursorOnRelease) {
            BsMenuMgr::s_pInstance->m_HandGrabTimer = m_CursorTimer - 1;
            BsMenuMgr::Get()->m_HandMode = 1;
        }
        return;
    }

    if (BsMenuMgr::Get()->m_HandMode == 1) {
        BsMenuMgr::Get()->m_pBsHandCursor->Disable();
    }
    nn::math::VEC2 screen(touch->lastX, touch->lastY);
    nn::math::VEC2 pos;
    m_pLayout->ScreenToLayout(screen, &pos);
    if (m_pBounding != NULL && !nw::lyt::IsContain(m_pBounding, pos)) {
        return;
    }

    if (!touch->prevIsTouch) {
        // the touch began: the last touched button becomes the current one, the others are unselected
        m_IsTouchOnButton = 1;
        m_pCurrent = NULL;
        for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
            if (node->IsTouch()) {
                m_pCurrent = node;
                m_PlayTouchSound = true;
            }
        }
        if (m_pCurrent == NULL) {
            m_IsTouchOnButton = 0;
            return;
        }
        for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
            if (m_pCurrent != node) {
                node->Select(false, false);
            }
        }
    } else if (m_pCurrent == NULL) {
        return;
    }

    if (m_IsTouchOnButton == 1) {
        m_pCurrent->TouchSelect(m_pCurrent->IsTouch(), m_PlayTouchSound);
        m_PlayTouchSound = false;
    }
}

// 0x0033F61C slot 0x14 (name after libgarden)
void ButtonActionControlChoice::ProcessOk()
{
    if (UpdateCurrentTouched()) {
        return;
    }
    if (CheckInputForNode()) {
        return;
    }
    CheckRelease();
    if ((m_Flags & OK_DONE) != 0) {
        return;
    }
    BsMenuMgr* mgr = BsMenuMgr::Get();
    if ((m_Flags2 & ENABLE_CURSOR_PERMANENT) == 0) {
        return;
    }
    if (mgr->m_HandMode == 0) {
        mgr->m_HandMode = 1;
    }
    m_CurrentIndex = BsMenuMgr::s_pInstance->m_CurrentActionNodeIndex;
    CheckInputForNodeOk();
    BsMenuMgr::s_pInstance->m_CurrentActionNodeIndex = m_CurrentIndex;
}

// 0x0033F6C4 (name after the RTTI)
ButtonActionControlChoice::ButtonActionControlChoice() : m_PlayTouchSound(false), m_IsTouchOnButton(0)
{
}

// 0x0033F720 slot 0x00 | slot vf_0x00 of ssys::st::List
// 0x0033F6E8 (deleting dtor)
ButtonActionControlChoice::~ButtonActionControlChoice()
{
    // empty (in the original too)
}
