#include "Button/dButtonActionControl.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dFormatBuffer.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundMgr.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Bounding.h"

namespace {
// the pad bit that chooses the current button (and keeps a repeating one going)
const u32 INPUT_OK = 4;
// the pad bits that make the cursor keys give up the cursor
const u32 INPUT_CANCEL_CURSOR = 0x74000;
// the cursor id of a cancelled cursor
const u8 CURSOR_CANCELLED = 12;
const u8 CURSOR_CANCELLED_DONE = 13;
// the frames until a held repeating button is chosen again
const s32 REPEAT_FIRST_DELAY = 11;
const s32 REPEAT_DELAY = 3;
} // namespace

// read by GetPressedIndex and IsSelectOkDone, but never written (name is ours)
// 0x00951878
u32 g_ButtonActionControlUnknown;

// 0x002F5EFC slot 0x1C (name after libgarden)
void ButtonActionControl::CheckInputForNodeOk()
{
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    if ((m_Flags & FLAG_100) == 0) {
        bool cancel = (m_Flags & CURSOR_KEYS) == 0 && mgr->IsPressed(INPUT_CANCEL_CURSOR);
        if (cancel || (m_pCurrent != NULL && !m_pCurrent->IsCursorHover())) {
            BsMenuMgr::s_pInstance->m_HandGrabTimer = CURSOR_CANCELLED;
            if (m_pCurrent != NULL) {
                m_pCurrent->Select(false, false);
                m_pCurrent = NULL;
            }
            return;
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

// 0x002F6188 slot 0x10 (name after libgarden)
void ButtonActionControl::Process()
{
    if ((m_Flags2 & ENABLE_TOUCH) == 0) {
        return;
    }
    TouchState* touch = &g_TouchState;
    if (!touch->isTouch) {
        // released: a button chosen by touch is chosen when the touch ends on it
        if (m_pCurrent == NULL) {
            return;
        }
        if (m_pCurrent->IsTouch()) {
            SelectCurrent();
            return;
        }
        m_pCurrent->TouchSelect(false, false);
        m_pCurrent = NULL;
        if (m_EnableCursorOnRelease) {
            BsMenuMgr::s_pInstance->m_HandGrabTimer = m_CursorTimer - 1;
            BsMenuMgr::Get()->m_HandMode = 1;
        }
        return;
    }

    if (BsMenuMgr::Get()->m_HandMode == 1) {
        BsMenuMgr::Get()->m_pBsHandCursor->Disable();
        if (m_pCurrent != NULL) {
            m_pCurrent->Select(false, false);
            m_pCurrent = NULL;
        }
    }
    if (touch->isTouch && !touch->prevIsTouch && m_pCurrent != NULL) {
        m_pCurrent->Select(false, false);
        m_pCurrent = NULL;
    }
    nn::math::VEC2 screen(touch->lastX, touch->lastY);
    nn::math::VEC2 pos;
    m_pLayout->ScreenToLayout(screen, &pos);
    if (m_pBounding != NULL && !nw::lyt::IsContain(m_pBounding, pos)) {
        if (m_pCurrent != NULL) {
            m_pCurrent->TouchSelect(false, false);
        }
        return;
    }

    if (!touch->prevIsTouch) {
        // the touch began: the last touched button becomes the current one
        m_pCurrent = NULL;
        for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
            if (node->IsTouch()) {
                m_pCurrent = node;
            } else {
                node->Select(false, false);
            }
        }
        if (m_pCurrent == NULL) {
            return;
        }
    } else if (m_pCurrent == NULL) {
        m_Counter = -1;
        m_MaxCounter = REPEAT_FIRST_DELAY;
        m_IsFirstRepeat = 1;
        return;
    }

    bool isTouch = m_pCurrent->IsTouch();
    if (m_pCurrent->m_IsPressed != isTouch) {
        m_pCurrent->TouchSelect(isTouch, true);
    }
    if (!isTouch) {
        m_Counter = -1;
        m_MaxCounter = REPEAT_FIRST_DELAY;
        m_IsFirstRepeat = 1;
    } else if (++m_Counter >= m_MaxCounter) {
        m_MaxCounter = REPEAT_DELAY;
        m_Counter = 0;
        m_IsFirstRepeat = 0;
    }
    if (m_pCurrent->m_IsRepeat && m_Counter == 0) {
        SelectCurrent();
    }
}

// 0x002F6498 slot 0x24 (name after libgarden)
void ButtonActionControl::CheckRelease()
{
    BsMenuMgr* mgr = BsMenuMgr::Get();
    if ((m_Flags2 & CAN_RELEASE) == 0) {
        return;
    }
    if (mgr->m_HandMode != 0 && BsMenuMgr::s_pInstance->m_HandGrabTimer != m_CursorTimer) {
        return;
    }
    if (!BsMenuMgr::s_pInstance->IsPressed(m_ReleaseInput)) {
        return;
    }
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        node->Select(false, false);
        node->TouchSelect(false, false);
    }
    m_pCurrent = NULL;
    SoundMgr::PlaySound(SE_SYS_CANCEL);
    m_Flags |= OK_DONE;
}

// 0x002F655C slot 0x20 (name after libgarden)
void ButtonActionControl::CheckInputForNodeTouch()
{
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    const sead::Vector2<f32>& stick = mgr->GetLeftStick();
    if (stick.x != 0.0f || stick.y != 0.0f) {
        BsMenuMgr::Get()->m_pBsHandCursor->Enable();
    }
    BsHandCursor* hand = BsMenuMgr::Get()->m_pBsHandCursor;
    nn::math::VEC2 pos = hand->m_Position;
    if (m_pBounding != NULL && !nw::lyt::IsContain(m_pBounding, pos)) {
        if (m_pCurrent != NULL) {
            m_pCurrent->Select(false, false);
            m_pCurrent = NULL;
        }
        return;
    }

    if (hand->m_IsEnabled) {
        // the button under the hand cursor becomes the current one
        u8 index = 0;
        ButtonActionNode* node = GetNextActive(NULL);
        while (node != NULL && !node->IsCursorHover()) {
            node = GetNextActive(node);
            index++;
        }
        if (m_pCurrent != node) {
            if (m_pCurrent != NULL) {
                m_pCurrent->Select(false, false);
            }
            if (node != NULL) {
                node->Select(true, true);
                m_CurrentIndex = index;
            }
            m_pCurrent = node;
        }
        if (m_pCurrent == NULL) {
            BsMenuMgr::Get()->m_pBsHandCursor->SetPressed(false);
            m_Counter = -1;
            m_MaxCounter = REPEAT_FIRST_DELAY;
            m_IsFirstRepeat = 1;
            return;
        }
        if (m_pCurrent->m_IsRepeat) {
            bool hold = mgr->IsHold(INPUT_OK);
            BsMenuMgr::Get()->m_pBsHandCursor->SetPressed(hold);
            if (hold) {
                if (++m_Counter >= m_MaxCounter) {
                    m_MaxCounter = REPEAT_DELAY;
                    m_Counter = 0;
                    m_IsFirstRepeat = 0;
                    SelectCurrent();
                    m_pCurrent->Animate(m_pCurrent->m_Touch, true);
                    return;
                }
                if (m_Counter == 0) {
                    SelectCurrent();
                    m_pCurrent->Animate(m_pCurrent->m_Touch, true);
                    return;
                }
            } else {
                m_Counter = -1;
                m_MaxCounter = REPEAT_FIRST_DELAY;
                m_IsFirstRepeat = 1;
            }
        }
    }

    bool ok = (m_Flags2 & WAIT_FOR_SELECT_RELEASE) != 0 ? mgr->IsRelease(INPUT_OK) : mgr->IsPressed(INPUT_OK);
    if (ok && m_pCurrent != NULL) {
        m_pCurrent->Animate(m_pCurrent->m_Touch, true);
        SelectCurrent();
    }
}

// 0x002F6928 | libgarden [tier A]
void ButtonActionControl::State_Selecting()
{
    if ((m_Flags & OK_DONE) != 0 && m_pCurrent != NULL && m_pCurrent->m_IsRepeat) {
        // a repeating button stays chosen while it is held
        if (g_TouchState.isTouch || BsMenuMgr::s_pInstance->IsHold(INPUT_OK)) {
            m_Flags &= ~OK_DONE;
            m_pCurrent->Select(true);
            m_pFunc = &ButtonActionControl::State_Normal;
            return;
        }
        Reset();
        return;
    }
    if ((m_Flags & OK_DONE) == 0 && m_pCurrent != NULL) {
        if (m_pCurrent->IsOkOngoing()) {
            m_pCurrent->DoOk();
        } else {
            m_Flags |= OK_DONE;
            FUN_002f6ee4();
            m_pCurrent->EndSelectOk();
        }
    }
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        node->FUN_002b72e0();
    }
    m_Flags &= ~IS_SELECTING_OK;
}

// 0x002F6AA4 (name is ours)
void ButtonActionControl::State_Normal()
{
    if ((m_Flags & DISABLED) == 0 && !((BsMenuMgr::s_Flags & 0x800) != 0 && (m_Flags & FLAG_80) != 0) &&
        (m_Flags & OK_DONE) == 0) {
        if (g_TouchState.isTouch || g_TouchState.prevIsTouch) {
            Process();
        } else {
            ProcessOk();
        }
    }
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        node->FUN_002b72e0();
    }
}

// 0x002F6B78 (name is ours)
void ButtonActionControl::BeginCursor()
{
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    mgr->m_HandGrabTimer = m_CursorTimer;
    if ((m_Flags2 & ENABLE_CURSOR) != 0) {
        const sead::Vector2<f32>& stick = mgr->GetLeftStick();
        if (stick.x != 0.0f || stick.y != 0.0f) {
            BsMenuMgr::Get()->m_pBsHandCursor->Enable();
        }
    }
    m_CurrentIndex = BsMenuMgr::s_pInstance->m_CurrentActionNodeIndex;
    if ((m_Flags & FLAG_100) == 0) {
        return;
    }
    u16 index = 0;
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        if (m_CurrentIndex == index) {
            node->Select(true, true);
            m_pCurrent = node;
            node->MoveHandCursor();
            return;
        }
        index++;
    }
    m_CurrentIndex = 0;
    if ((m_Flags & FLAG_20) != 0) {
        BsMenuMgr::s_pInstance->m_CurrentActionNodeIndex = 0;
    }
    ButtonActionNode* node = GetNextActive(NULL);
    if (node != NULL) {
        node->MoveHandCursor();
    }
}

// 0x002F6D30 (name is ours)
void ButtonActionControl::ResetSelectByIndex(s32 index)
{
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        if (node->m_Index == index) {
            node->ResetSelect();
        }
    }
}

// 0x002F6DA4 | libgarden [tier A]
void ButtonActionControl::UnselectCurrent()
{
    if (m_pCurrent != NULL) {
        m_pCurrent->TouchSelect(false, false);
        m_pCurrent = NULL;
    }
}

// 0x002F6DD8 slot 0x34 (name after libgarden)
void ButtonActionControl::OnBegin()
{
    // empty (in the original too)
}

// 0x002F6DDC slot 0x18 (name after libgarden)
bool ButtonActionControl::CheckInputForNode()
{
    // (the original calls it and drops the result)
    BsMenuMgr::Get();
    if ((m_Flags2 & ENABLE_NODE_SHORTCUT) == 0) {
        return false;
    }
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    if ((m_Flags & ENABLE_BUTTON_SHORTCUT) == 0 && mgr->m_HandGrabTimer != m_CursorTimer &&
        mgr->m_HandGrabTimer != m_CursorTimer - 1) {
        return false;
    }
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        if (BsMenuMgr::s_pInstance->IsPressed(node->m_Input)) {
            if (m_pCurrent != NULL) {
                m_pCurrent->Select(false, false);
            }
            m_pCurrent = node;
            node->TouchSelect(true, false);
            SelectCurrent();
            return true;
        }
    }
    return false;
}

// 0x002F6EE4 slot 0x30 (name after libgarden)
void ButtonActionControl::FUN_002f6ee4()
{
    // empty (in the original too)
}

// 0x002F6EE8 slot 0x2C (name after libgarden)
void ButtonActionControl::SelectCurrent()
{
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        if (m_pCurrent != node) {
            node->Select(false, false);
        }
    }
    m_pCurrent->BeginSelectOk();
    m_Flags |= IS_SELECTING_OK;
    m_pFunc = &ButtonActionControl::State_Selecting;
}

// 0x002F6F90 (name is ours)
void ButtonActionControl::SetNodeActive(s32 index, bool active)
{
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        if (index == -1 || node->m_Index == index) {
            node->m_IsActive = active;
        }
    }
}

// 0x002F70EC (name is ours)
void ButtonActionControl::SelectNode(s32 index, bool select)
{
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        if (index == -1 || node->m_Index == index) {
            node->Select(select, false);
            m_pCurrent = select ? node : NULL;
        }
    }
}

// 0x002F7154 (libgarden)
void ButtonActionControl::SetCursor(bool enable)
{
    SetFlags(CURSOR_KEYS, enable);
    // the pad bits of the cursor keys (the first control decides for all)
    // 0x00951880 (guard 0x0095187C)
    static u32 s_CursorBits = (m_CursorInput & 0x1000) != 0 ? 0x60000 : 0x14000;
    if ((m_Flags & CURSOR_KEYS) != 0) {
        m_CursorInput |= s_CursorBits;
    } else {
        m_CursorInput &= ~s_CursorBits;
    }
}

// 0x002F732C (name is ours)
void ButtonActionControl::ResetSelectAll()
{
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        node->ResetSelect();
    }
}

// 0x002F7300 slot 0x38 (name after libgarden)
void ButtonActionControl::FUN_002f7300()
{
    // empty (in the original too)
}

// 0x002F7394 | libgarden [tier A]
void ButtonActionControl::UnselectActive()
{
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        node->TouchSelect(false, false);
    }
    m_pCurrent = NULL;
}

// 0x002F743C (name is ours)
void ButtonActionControl::InitializeCursor(bool enable)
{
    m_Flags &= ~FLAG_4;
    m_CursorInput = 0xC04;
    SetCursor(enable);
}

// 0x002F74C4 slot 0x28 (name after libgarden)
bool ButtonActionControl::UpdateCurrentTouched()
{
    if ((m_Flags2 & NODE_OK_IMMEDIATE) == 0) {
        return false;
    }
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        if (node->m_IsOkRequested) {
            if (m_pCurrent != NULL) {
                m_pCurrent->TouchSelect(false, false);
            }
            m_pCurrent = node;
            node->TouchSelect(true, false);
            SelectCurrent();
            return true;
        }
    }
    return false;
}

// 0x002F7654 (name is ours)
void ButtonActionControl::SelectByIndex(s32 index)
{
    if (m_pCurrent != NULL) {
        m_pCurrent->Select(false);
    }
    m_pCurrent = NULL;
    if (index == -1) {
        return;
    }
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        if (node->m_Index == index) {
            node->Select(true, false);
            m_pCurrent = node;
            return;
        }
    }
}

// 0x002F7700 (name is ours)
void ButtonActionControl::RequestOk(s32 index, u8 request)
{
    for (ButtonActionNode* node = static_cast<ButtonActionNode*>(m_pFirst); node != NULL;
         node = static_cast<ButtonActionNode*>(node->m_pNext)) {
        if (index == -1 || node->m_Index == index) {
            node->m_IsOkRequested = request;
            m_pCurrent = node;
        }
    }
}

// 0x002F779C | libgarden [tier A]
void ButtonActionControl::Reset()
{
    if ((m_Flags & (OK_DONE | IS_SELECTING_OK)) != 0) {
        OnBegin();
        m_pCurrent = NULL;
        m_Flags &= ~(OK_DONE | IS_SELECTING_OK);
        m_pFunc = &ButtonActionControl::State_Normal;
    }
}

// 0x002F7810 | libgarden [tier A]
void ButtonActionControl::Initialize(ButtonActionControl::Description const& desc, char const* animNamePrefix)
{
    m_CursorTimer = desc.cursorTimer;
    m_pLayout = desc.layout;
    m_pCheckInput = desc.checkInput;
    m_pBounding = desc.bounding;
    m_Flags = FLAG_4 | DISABLED;
    if (animNamePrefix != NULL) {
        // the animations "<prefix>_touch.bclan" ...
        g_FormatBuffer.format("%s_%s.bclan", animNamePrefix, "touch");
        m_Touch.Initialize(g_FormatBuffer.getCString(), desc.resAcc);
        g_FormatBuffer.format("%s_%s.bclan", animNamePrefix, "select");
        m_Select.Initialize(g_FormatBuffer.getCString(), desc.resAcc);
        g_FormatBuffer.format("%s_%s.bclan", animNamePrefix, "touch_ok");
        m_TouchOk.Initialize(g_FormatBuffer.getCString(), desc.resAcc);
        g_FormatBuffer.format("%s_%s.bclan", animNamePrefix, "select_ok");
        m_SelectOk.Initialize(g_FormatBuffer.getCString(), desc.resAcc);
    } else {
        if (desc.touch != NULL) {
            m_Touch.Initialize(desc.touch, desc.resAcc);
        }
        if (desc.select != NULL) {
            m_Select.Initialize(desc.select, desc.resAcc);
        }
        if (desc.touchOk != NULL) {
            m_TouchOk.Initialize(desc.touchOk, desc.resAcc);
        }
        if (desc.selectOk != NULL) {
            m_SelectOk.Initialize(desc.selectOk, desc.resAcc);
        }
    }
    // (the original calls it and drops the result)
    BsMenuMgr::Get();
}

// 0x002F7BA8 slot 0x08 | libgarden
void ButtonActionControl::Update()
{
    (this->*m_pFunc)();
}

// 0x002F7BCC slot 0x14 (name after libgarden)
void ButtonActionControl::ProcessOk()
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
    BsHandCursor* hand = mgr->m_pBsHandCursor;
    if ((m_Flags2 & (ENABLE_CURSOR | ENABLE_CURSOR_PERMANENT)) == 0) {
        return;
    }
    if (mgr->m_HandMode == 0) {
        // the hand cursor comes with the cursor input, a move of the circle pad or the flag
        bool pressed = BsMenuMgr::s_pInstance->IsPressed(m_CursorInput);
        bool moving = false;
        if ((m_Flags2 & ENABLE_CURSOR) != 0) {
            moving = hand->m_Move.x != 0.0f || hand->m_Move.y != 0.0f;
        }
        if (pressed | moving | m_EnableCursorOnRelease) {
            BsMenuMgr::s_pInstance->m_HandGrabTimer = m_CursorTimer - 1;
            mgr->m_HandMode = 1;
        }
        return;
    }
    u8 owner = BsMenuMgr::s_pInstance->m_HandGrabTimer;
    if (owner == m_CursorTimer - 1) {
        BeginCursor();
        return;
    }
    if (owner == CURSOR_CANCELLED) {
        BsMenuMgr::s_pInstance->m_HandGrabTimer = CURSOR_CANCELLED_DONE;
        return;
    }
    if (owner == m_CursorTimer && (m_Flags2 & ENABLE_CURSOR_PERMANENT) != 0) {
        CheckInputForNodeOk();
    } else if ((m_Flags2 & ENABLE_CURSOR) != 0) {
        CheckInputForNodeTouch();
    }
    BsMenuMgr::s_pInstance->m_CurrentActionNodeIndex = m_CurrentIndex;
}

// 0x002F7F30 | libgarden [tier A]
void ButtonActionControl::AddNode(ButtonActionNode* node, bool updateAnimations)
{
    if (updateAnimations) {
        node->SetAnimTransforms(m_Touch.GetAnimTransform(), m_Select.GetAnimTransform(), m_TouchOk.GetAnimTransform(),
                                m_SelectOk.GetAnimTransform(), m_pLayout);
    }
    PushBack(node);
}

// 0x002F7FD0 | libgarden [tier A]
ButtonActionControl::ButtonActionControl()
    : m_pLayout(NULL), m_pBounding(NULL), m_pCurrent(NULL), m_pFunc(&ButtonActionControl::State_Normal),
      m_pCheckInput(NULL), m_CursorInput(INPUT_OK), m_ReleaseInput(8), m_CursorTimer(0),
      m_Flags2(ENABLE_TOUCH | ENABLE_CURSOR | ENABLE_CURSOR_PERMANENT | ENABLE_NODE_SHORTCUT | CAN_RELEASE),
      m_CurrentIndex(0), m_EnableCursorOnRelease(false), m_Flags(0), m_Counter(-1), m_MaxCounter(REPEAT_FIRST_DELAY),
      m_IsFirstRepeat(1)
{
}

// 0x002F8188 slot 0x00 | libgarden
// 0x002F80A4 (deleting dtor)
ButtonActionControl::~ButtonActionControl()
{
    // (only the members are destroyed)
}

// 0x007241B0 | libgarden [tier A]
s32 ButtonActionControl::GetPressedIndex() const
{
    if ((m_Flags & OK_DONE) != 0 ||
        (g_ButtonActionControlUnknown != 0 && m_pFunc == &ButtonActionControl::State_Selecting)) {
        return m_pCurrent != NULL ? m_pCurrent->m_Index : -2;
    }
    return -1;
}

// 0x00724214 | libgarden [tier A]
s32 ButtonActionControl::GetSelectedIndex() const
{
    if (m_pFunc == &ButtonActionControl::FUN_002f7300) {
        return -1;
    }
    if (m_pCurrent != NULL) {
        return m_pCurrent->m_Index;
    }
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL;
         node = GetNextActive(node)) {
        if (node->m_IsPressed == true) {
            return node->m_Index;
        }
    }
    return -1;
}

// 0x00724498 | libgarden [tier A]
bool ButtonActionControl::IsSelectOkDone() const
{
    if ((m_Flags & OK_DONE) != 0) {
        return true;
    }
    if (g_ButtonActionControlUnknown != 0 && m_pFunc == &ButtonActionControl::State_Selecting) {
        return true;
    }
    return false;
}
