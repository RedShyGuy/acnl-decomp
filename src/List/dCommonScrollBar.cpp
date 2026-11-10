#include "List/dCommonScrollBar.h"

#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "List/dContentNode.h"
#include "Other/dController.h"
#include "Other/dMenuBase.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nn/math/math_ARMv6.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Pane.h"

namespace {
// the pad bits of the menus the scroll bar reacts to (the cursor keys and the circle pad)
const u32 PAD_UP = 1 << 10;
const u32 PAD_DOWN = 1 << 11;
const u32 PAD_LEFT = 1 << 12;
const u32 PAD_RIGHT = 1 << 13;
// the pad bit that grabs the thumb with the hand cursor
const u32 PAD_GRAB = 1 << 2;
// the flag of ButtonActionControl::m_Flags the scroll bar sets while it has the input
const u32 CONTROL_FLAG_BLOCKED = 1 << 3;

// (inline) the hand cursor of the menus
inline BsHandCursor* GetHandCursor()
{
    return BsMenuMgr::Get()->m_pBsHandCursor;
}
} // namespace

// 0x00133ACC | libgarden [tier A]
void CommonScrollBar::Finalize()
{
    SetThumbSelected(false);
    nw::lyt::Pane* root = m_Layout.GetRootPane();
    if (root->m_pParent != NULL) {
        root->m_pParent->RemoveChild(root);
    }
    m_Layout.UnbindAllAnimation();
    m_ArcReader.Clear();
    BsHandCursor* cursor = GetHandCursor();
    cursor->m_Unknown0x4D7 = false;
    if (m_Unknown0x701 == true) {
        cursor->Disable();
    }
    m_Contents.Clear();
    m_Control.Clear();
}

// 0x00297E3C
void CommonScrollBar::UpdatePad()
{
    if (m_ItemNum == 0 || m_Mode != MODE_PAD || !m_IsPadEnabled) {
        return;
    }
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    if (m_IsScrollByPad == true) {
        // the cursor keys scroll by one with the repeat of the key
        if (m_Repeat.m_Counter != 0) {
            return;
        }
        if (mgr->IsHold(PAD_UP)) {
            if (m_Scroll <= 0) {
                return;
            }
            Scroll(m_Scroll - 1);
            SoundMgr::PlaySound(m_IsHorizontal ? SE_SYS_SCROLL_BTN_L_REPEAT : SE_SYS_SCROLL_BTN_U_REPEAT, m_UpArrowPan);
            return;
        }
        if (m_Scroll >= m_MaxScroll) {
            return;
        }
        Scroll(m_Scroll + 1);
        SoundMgr::PlaySound(m_IsHorizontal ? SE_SYS_SCROLL_BTN_R_REPEAT : SE_SYS_SCROLL_BTN_D_REPEAT, m_DownArrowPan);
        return;
    }

    // the cursor keys move the selection; the list scrolls when it leaves the visible items
    if (m_OnSelect == NULL) {
        return;
    }
    const s32 prev = m_Selected;
    if (prev != -1 && m_Scroll <= prev && static_cast<u32>(prev) < static_cast<u32>(m_Scroll + m_VisibleNum)) {
        if (m_Repeat.m_Counter == 0) {
            if (mgr->IsHold(PAD_UP)) {
                if (m_Selected > 0) {
                    m_Selected--;
                }
                if (m_Selected < m_Scroll) {
                    Scroll(m_Scroll - 1);
                }
            } else if (BsMenuMgr::s_pInstance->IsHold(PAD_DOWN)) {
                if (static_cast<u32>(m_Selected) < static_cast<u32>(m_ItemNum - 1)) {
                    m_Selected++;
                }
                const s32 offset = m_Selected - m_Scroll;
                if (static_cast<u32>(offset) >= static_cast<u32>(m_VisibleNum - 1)) {
                    if (offset == m_VisibleNum) {
                        Scroll(m_Scroll + 2);
                    } else {
                        Scroll(m_Scroll + 1);
                    }
                }
            }
        }
    } else if (mgr->IsHold(PAD_UP) || BsMenuMgr::s_pInstance->IsHold(PAD_DOWN)) {
        // the selection is not visible: it starts at the top
        m_Selected = m_Scroll;
    }
    if (m_Selected != prev) {
        m_OnSelect(m_pContainer, m_Selected, prev);
        SoundMgr::PlaySound(m_IsAudioSelectSound ? SE_SYS_AUDIO_TITLE_SELECTED : SE_SYS_SCROLL_LIST_SELECTED, 0.0f);
    }
}

// 0x0029811C | libgarden [tier A]
void CommonScrollBar::SetParent(nw::lyt::Pane* parent)
{
    nw::lyt::Pane* root = m_Layout.GetRootPane();
    if (root->m_pParent != NULL) {
        root->m_pParent->RemoveChild(root);
    }
    if (parent != NULL) {
        parent->AppendChild(m_Layout.GetRootPane());
        m_Layout.Animate();
        m_ThumbPan = GetPaneSoundPan(parent);
        m_UpArrowPan = GetPaneSoundPan(m_Layout.FindPane("B_triT_00"));
        m_DownArrowPan = GetPaneSoundPan(m_Layout.FindPane("B_triB_00"));
    }
}

// 0x00298198 slot 0x18
void CommonScrollBar::UpdateContents()
{
    UpdateScrollFromThumb();
    for (ContentNode* node = static_cast<ContentNode*>(m_Contents.GetFirst()); node != NULL;
         node = static_cast<ContentNode*>(node->GetNext())) {
        nw::lyt::Pane* pane = node->m_pPane;
        pane->SetTranslate(nn::math::VEC3(pane->m_Translate.x, node->m_BasePosition.y + m_ScrollRemainder, pane->m_Translate.z));
    }
}

// 0x002981FC
void CommonScrollBar::SetArrowNg(bool isNg, s32 index)
{
    ButtonActionNode& arrow = m_Arrows[index];
    nw::lyt::Group* group = index == 0 ? m_pUpArrowGroup : m_pDownArrowGroup;
    if (arrow.IsActivated() != isNg) {
        return;
    }
    m_Layout.BindAnimation(&m_NgAnim, group, false);
    if (isNg) {
        m_NgAnim.SetFrame(0.0f);
        m_Control.UnselectCurrent();
        arrow.Select(false, false);
        arrow.m_IsActive = false;
        if (m_TouchAnim.m_IsBound) {
            m_TouchAnim.SetFrame(0.0f);
            m_Layout.Animate();
            m_Layout.UnbindAnimation(&m_TouchAnim);
        }
    } else {
        m_NgAnim.SetFrame(1.0f);
        arrow.m_IsActive = true;
    }
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&m_NgAnim, group, false);
}

// 0x00298310
void CommonScrollBar::UpdateAllVisible()
{
    m_pAll->m_Flag = (m_pAll->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) |
                     (static_cast<u32>(m_ItemNum) > static_cast<u32>(m_VisibleNum - 1) ? 1 : 0);
}

// 0x00298340 slot 0x0C
void CommonScrollBar::UpdateTouch()
{
    if (m_Mode != MODE_TOUCH) {
        return;
    }
    UpdateArrowButtons();
    if (m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing()) {
        return;
    }
    if (Controller::IsTrig(Controller::TOUCH)) {
        nn::math::VEC2 pos;
        m_Layout.ScreenToLayout(reinterpret_cast<const nn::math::VEC2&>(Controller::GetPointer()), &pos);
        if (IsOnThumb(pos)) {
            // the thumb is grabbed
            m_IsDragging = true;
            SetThumbSelected(true);
            const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
            m_GrabOffset = thumb.y - pos.y;
            SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_ACTIVE, m_ThumbPan);
        } else if (nw::lyt::IsContain(m_pBar, pos)) {
            OnTouchTrack(pos);
        }
    } else if (Controller::IsRelease(Controller::TOUCH) && m_IsDragging == true) {
        EndDrag();
    }
}

// 0x002984C0
void CommonScrollBar::SetItemNum(u32 num, bool isKeepScroll)
{
    if (m_Unknown0x754 <= num) {
        m_Unknown0x754 = num;
    }
    m_ItemNum = num;
    UpdateMaxScroll();
    UpdateAllVisible();
    UpdateThumbSize();
    UpdateThumbBase();
    s32 scroll = 0;
    if (isKeepScroll) {
        scroll = m_Scroll;
        if (scroll > m_MaxScroll) {
            scroll = m_MaxScroll;
        }
    }
    Scroll(scroll);
}

// 0x0029857C
void CommonScrollBar::UpdateHandCursor()
{
    if (!GetHandCursor()->m_IsEnabled || m_Mode != MODE_HAND_CURSOR || m_IsDragging == true) {
        return;
    }
    UpdateArrowButtons();
    if (m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing()) {
        return;
    }
    const nn::math::VEC2 pos = GetHandCursor()->m_Position;
    if (IsOnThumb(pos)) {
        SetThumbSelected(true);
    } else {
        SetThumbSelected(false);
    }
    if (!BsMenuMgr::s_pInstance->IsPressed(PAD_GRAB)) {
        return;
    }
    if (m_IsThumbSelected) {
        // the thumb is grabbed with the cursor
        GetHandCursor()->Func1F9FC4();
        MoveHandCursorToThumb();
        m_Control.m_Flags |= CONTROL_FLAG_BLOCKED;
        m_GrabOffset = 0.0f;
        m_IsDragging = true;
        SetThumbSelected(true);
        SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_ACTIVE, m_ThumbPan);
    } else if (nw::lyt::IsContain(m_pBar, pos)) {
        GetHandCursor()->Func1F9FC4();
        MoveHandCursorToThumb();
        m_Control.m_Flags |= CONTROL_FLAG_BLOCKED;
        m_GrabOffset = 0.0f;
        OnTouchTrack(pos);
    }
}

// 0x00298768
void CommonScrollBar::UpdateDrag()
{
    if (!m_IsDragging || m_IsTrackTouched == true) {
        return;
    }
    nn::math::VEC2 pos;
    if (m_Mode == MODE_TOUCH) {
        if (!g_TouchState.IsHeld()) {
            return;
        }
        m_Layout.ScreenToLayout(reinterpret_cast<const nn::math::VEC2&>(Controller::GetPointer()), &pos);
    } else if (m_Mode == MODE_HAND_CURSOR) {
        if (BsMenuMgr::s_pInstance->IsRelease(PAD_GRAB)) {
            GetHandCursor()->Func1FA0C4(false);
            m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
            EndDrag();
            return;
        }
        pos = GetHandCursor()->m_Position;
        const BsHandCursor* cursor = GetHandCursor();
        if (cursor->m_Move.x == 0.0f && cursor->m_Move.y == 0.0f) {
            return;
        }
    } else {
        return;
    }
    OnDragMove(pos);
}

// 0x002988AC
void CommonScrollBar::Scroll(s32 pos)
{
    m_Scroll = pos;
    if (m_Scroll > m_MaxScroll) {
        m_Scroll = m_MaxScroll;
    } else if (m_Scroll <= 0) {
        m_Scroll = 0;
    }
    const f32 offset = -(m_Scroll * m_ScrollSpace) / m_ScrollPerUnit;
    m_pThumb->SetTranslate(nn::math::VEC3(m_pThumb->m_Translate.x, m_ThumbTop + offset, m_pThumb->m_Translate.z));
    for (ContentNode* node = static_cast<ContentNode*>(m_Contents.GetFirst()); node != NULL;
         node = static_cast<ContentNode*>(node->GetNext())) {
        node->m_pPane->SetTranslate(node->m_BasePosition);
    }
    m_ScrollRemainder = 0.0f;

    // a button that was being chosen finishes first
    if (m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing()) {
        do {
            m_Control.Update();
        } while (!m_Control.IsSelectOkDone());
        m_Control.UnselectCurrent();
        m_Control.Reset();
    }
    if (m_Scroll == 0) {
        SetArrowNg(true, 0);
        SetArrowNg(false, 1);
    } else if (m_Scroll == m_MaxScroll) {
        SetArrowNg(false, 0);
        SetArrowNg(true, 1);
    } else {
        SetArrowNg(false, 0);
        SetArrowNg(false, 1);
    }
    if (m_OnScroll != NULL) {
        m_OnScroll(m_pContainer);
    }
    m_SoundTop = m_HalfSpace + m_Scroll * m_ScrollSpace;
    m_SoundBottom = m_Scroll * m_ScrollSpace - m_HalfSpace;
    if (m_IsDragging == true) {
        if (m_Mode == MODE_HAND_CURSOR) {
            GetHandCursor()->Func1FA0C4(false);
            m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
        }
        EndDrag();
    }
}

// 0x00298AF0
void CommonScrollBar::UpdateRepeat()
{
    bool isHeld = false;
    switch (m_Mode) {
    case MODE_PAD: {
        BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
        if ((mgr->IsHold(PAD_UP) || BsMenuMgr::s_pInstance->IsHold(PAD_DOWN)) && m_IsPadEnabled) {
            isHeld = true;
        }
        break;
    }
    case MODE_TOUCH:
        if (m_Control.IsCurrentPressed()) {
            isHeld = true;
        }
        // fall through
    case MODE_HAND_CURSOR:
        if (BsMenuMgr::s_pInstance->IsHold(PAD_GRAB) && m_Control.IsCurrentPressed()) {
            // an arrow is pressed with the cursor
            isHeld = true;
            if (!m_TouchAnim.m_IsBound) {
                m_Layout.BindAnimation(&m_TouchAnim, m_Control.GetSelectedIndex() == 0 ? m_pUpArrowGroup : m_pDownArrowGroup, false);
                m_TouchAnim.SetFrame(1.0f);
            }
            GetHandCursor()->SetPressed(true);
        } else if (m_TouchAnim.m_IsBound == 1) {
            GetHandCursor()->SetPressed(false);
            if (m_TouchAnim.m_IsBound) {
                m_TouchAnim.SetFrame(0.0f);
                m_Layout.Animate();
                m_Layout.UnbindAnimation(&m_TouchAnim);
            }
        }
        break;
    default:
        break;
    }
    m_Repeat.Update(isHeld);
}

// 0x00298CAC
void CommonScrollBar::UpdateThumbSize()
{
    if ((m_pAll->m_Flag & nw::lyt::Pane::FLAG_VISIBLE) == 0) {
        return;
    }
    const f32 length = m_BarTop - m_BarBottom;
    const f32 unit = length / static_cast<u32>(m_ItemNum);
    f32 height = unit * static_cast<u32>(m_VisibleNum - 1);
    if (height < 1.0f) {
        height = 0.0f;
    }
    m_ScrollPerUnit = static_cast<u32>(m_ItemNum - m_VisibleNum + 1) * m_ScrollSpace / (length - height);

    // the center of the thumb gets the height, the ends follow it
    const f32 width = m_pThumbCenterShadow->m_Size.width;
    m_pThumbCenterShadow->m_Size = nw::lyt::Size(width, height);
    m_pThumbCenter->m_Size = nw::lyt::Size(width, height);
    m_pThumbCenterBounding->m_Size = nw::lyt::Size(width, height);
    const f32 half = height * 0.5f;
    const nn::math::VEC3 center = m_pThumbCenterShadow->m_Translate;
    m_pThumbTopShadow->SetTranslate(nn::math::VEC3(center.x, center.y + half, center.z));
    m_pThumbTopBounding->SetTranslate(nn::math::VEC3(center.x, center.y + half, center.z));
    m_pThumbBottomShadow->SetTranslate(nn::math::VEC3(center.x, center.y - half, center.z));
    m_pThumbBottomBounding->SetTranslate(nn::math::VEC3(center.x, center.y - half, center.z));
    m_Layout.Animate();
}

// 0x00298DF0
bool CommonScrollBar::IsOnBar(const nn::math::VEC2& pos)
{
    return nw::lyt::IsContain(m_pBar, pos);
}

// 0x00298E08 slot 0x14
void CommonScrollBar::MoveThumb(f32 dy)
{
    m_ThumbMove = dy;
    ClampThumbMove();
}

// 0x00298E14
void CommonScrollBar::ClampThumbMove()
{
    if (m_ThumbMove == 0.0f) {
        return;
    }
    const f32 top = m_pThumbTopBounding->m_Translate.y + m_pThumb->m_Translate.y;
    const f32 bottom = m_pThumbBottomBounding->m_Translate.y + m_pThumb->m_Translate.y;
    if (m_ThumbMove > 0.0f) {
        if (m_BarTop < m_ThumbMove + top) {
            // the thumb reaches the top
            m_ThumbMove = m_BarTop - top;
            SetArrowNg(true, 0);
            SetArrowNg(false, 1);
            return;
        }
    } else if (m_BarBottom > m_ThumbMove + bottom) {
        // the thumb reaches the bottom
        m_ThumbMove = m_BarBottom - bottom;
        SetArrowNg(false, 0);
        SetArrowNg(true, 1);
        return;
    }
    SetArrowNg(false, 0);
    SetArrowNg(false, 1);
}

// 0x00298F0C
void CommonScrollBar::UpdateArrowButtons()
{
    if (m_IsBoard == true || m_Repeat.m_Counter != 0) {
        return;
    }
    const s32 index = m_Control.GetSelectedIndex();
    if (index == 0) {
        if (m_Scroll <= 0) {
            return;
        }
        Scroll(m_Scroll - 1);
        PlayUpSound(!m_Repeat.m_IsFirst);
    } else if (index == 1) {
        if (m_Scroll >= m_MaxScroll) {
            return;
        }
        Scroll(m_Scroll + 1);
        PlayDownSound(!m_Repeat.m_IsFirst);
    }
}

// 0x00299028
void CommonScrollBar::InitializeButtons()
{
    // copied from the image 0x00858BCC
    const char* const arrowPanes[2] = {"B_triT_00", "B_triB_00"};
    const char* const arrowGroups[2] = {"G_triT_00", "G_triB_00"};
    ButtonActionControl::Description desc = {
        &m_ArcReader,
        &m_Layout,
        NULL,
        "scrl_bar_touch.bclan",
        "scrl_bar_select.bclan",
        "scrl_bar_touch_ok.bclan",
        "scrl_bar_select_ok.bclan",
        3,
        NULL,
    };
    m_Control.Initialize(desc, NULL);
    m_Control.m_Flags |= 0x40;
    ButtonActionNode::Info info;
    info.pane = NULL;
    info.group = NULL;
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.input = 0;
    info.index = 0;
    info.cursorCallback = NULL;
    for (s32 i = 0; i < 2; i++) {
        info.pane = m_Layout.FindPane(arrowPanes[i]);
        info.group = m_Layout.FindGroup(arrowGroups[i]);
        ButtonActionNode& arrow = m_Arrows[i];
        info.index = i;
        info.input = 0;
        arrow.Initialize(info);
        arrow.m_OkSoundEnabled = false;
        arrow.m_SoundEnabled = false;
        m_Control.AddNode(&arrow, true);
    }
    m_Control.SetCursor(false);
    m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
    m_Control.m_Flags2 = 35;
}

// 0x0029916C
void CommonScrollBar::MoveHandCursorToThumb()
{
    const nn::math::VEC3 pos = GetGlobalPosition(m_pThumb);
    BsHandCursor* cursor = GetHandCursor();
    cursor->m_Position.x = pos.x;
    cursor->m_Position.y = pos.y;
    GetHandCursor()->SetPosition(&pos);
}

// 0x002991CC slot 0x08
void CommonScrollBar::OnTouchTrack(const nn::math::VEC2& pos)
{
    m_IsTrackTouched = true;
    SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_COME, m_ThumbPan);
    const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
    MoveThumb(pos.y - thumb.y);
    m_IsDragging = true;
    SetThumbSelected(true);
    m_GrabOffset = 0.0f;
}

// 0x00299260
void CommonScrollBar::UpdateMode()
{
    if (Controller::IsHold(Controller::TOUCH) || g_TouchState.prevIsTouch) {
        if (m_Mode != MODE_TOUCH) {
            LeaveMode();
        }
        m_Mode = MODE_TOUCH;
        return;
    }
    if (!m_IsDragging) {
        BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
        if ((mgr->IsHold(PAD_UP) || BsMenuMgr::s_pInstance->IsHold(PAD_DOWN) || BsMenuMgr::s_pInstance->IsHold(PAD_RIGHT) ||
             BsMenuMgr::s_pInstance->IsHold(PAD_LEFT)) &&
            !m_Control.IsSelectOkDone() && !m_Control.IsSelectOkOngoing() && m_IsPadEnabled) {
            if (m_Mode != MODE_PAD) {
                LeaveMode();
                if (m_Control.HasCurrent()) {
                    m_Control.UnselectCurrent();
                }
                m_Control.m_Flags |= CONTROL_FLAG_BLOCKED;
                BsHandCursor* cursor = GetHandCursor();
                cursor->m_Unknown0x4D7 = true;
                cursor->Disable();
                m_IsPadModeEntered = true;
            }
            m_Mode = MODE_PAD;
            return;
        }
    }
    if (GetHandCursor()->m_IsEnabled) {
        if (m_Mode != MODE_HAND_CURSOR) {
            LeaveMode();
        }
        m_Mode = MODE_HAND_CURSOR;
    } else {
        if (m_Mode != MODE_NONE) {
            LeaveMode();
        }
        m_Mode = MODE_NONE;
    }
}

// 0x00299450
void CommonScrollBar::PlayUpSound(bool isRepeat)
{
    SeID id = isRepeat ? SE_SYS_SCROLL_BTN_U_REPEAT : SE_SYS_SCROLL_BTN_U;
    if (m_IsHorizontal) {
        id = isRepeat ? SE_SYS_SCROLL_BTN_L_REPEAT : SE_SYS_SCROLL_BTN_L;
    }
    SoundMgr::PlaySound(id, m_UpArrowPan);
}

// 0x00299494
bool CommonScrollBar::IsOnThumb(const nn::math::VEC2& pos)
{
    return nw::lyt::IsContain(m_pThumbCenterBounding, pos) || nw::lyt::IsContain(m_pThumbTopBounding, pos) ||
           nw::lyt::IsContain(m_pThumbBottomBounding, pos);
}

// 0x002994E0
void CommonScrollBar::SetUnknown0x754(u32 value)
{
    m_Unknown0x754 = value;
}

// 0x002994E8
void CommonScrollBar::UpdateScrollFromThumb()
{
    const s32 prev = m_Scroll;
    const f32 distance = (m_ThumbTop - m_pThumb->m_Translate.y) * m_ScrollPerUnit;
    m_Scroll = static_cast<s32>((distance + 1.0f) / m_ScrollSpace);
    m_ScrollRemainder = distance - m_Scroll * m_ScrollSpace;
    if (m_ThumbMove < 0.0f) {
        if (!(m_SoundTop > distance)) {
            SoundMgr::SPlaySystemSound(SE_SYS_SCROLL_BAR_DRAG, GetPaneSoundPan(m_pThumb));
            m_SoundTop = m_HalfSpace + m_ScrollSpace + m_Scroll * m_ScrollSpace;
            m_SoundBottom = m_HalfSpace + m_Scroll * m_ScrollSpace;
        }
    } else if (!(m_SoundBottom < distance)) {
        SoundMgr::SPlaySystemSound(SE_SYS_SCROLL_BAR_DRAG, GetPaneSoundPan(m_pThumb));
        m_SoundTop = m_HalfSpace + m_Scroll * m_ScrollSpace;
        m_SoundBottom = m_Scroll * m_ScrollSpace - m_HalfSpace;
    }
    if (m_OnScroll != NULL && m_Scroll != prev) {
        m_OnScroll(m_pContainer);
    }
}

// 0x0029962C
void CommonScrollBar::UpdateMaxScroll()
{
    m_MaxScroll = m_ItemNum - m_VisibleNum + 1;
    if (m_MaxScroll <= 0) {
        m_MaxScroll = 0;
    }
}

// 0x00299650
void CommonScrollBar::PlayDownSound(bool isRepeat)
{
    SeID id = isRepeat ? SE_SYS_SCROLL_BTN_D_REPEAT : SE_SYS_SCROLL_BTN_D;
    if (m_IsHorizontal) {
        id = isRepeat ? SE_SYS_SCROLL_BTN_R_REPEAT : SE_SYS_SCROLL_BTN_R;
    }
    SoundMgr::PlaySound(id, m_DownArrowPan);
}

// 0x00299694
void CommonScrollBar::FindPanes()
{
    m_pThumb = m_Layout.FindPane("N_btn_00");
    m_pThumbCenter = m_Layout.FindPane("P_btnC_00");
    m_pThumbTopShadow = m_Layout.FindPane("P_btnT_sh_00");
    m_pThumbCenterShadow = m_Layout.FindPane("P_btnC_sh_00");
    m_pThumbBottomShadow = m_Layout.FindPane("P_btnB_sh_00");
    m_pThumbTopBounding = m_Layout.FindPane("B_btnT_00");
    m_pThumbCenterBounding = m_Layout.FindPane("B_btnC_00");
    m_pThumbBottomBounding = m_Layout.FindPane("B_btnB_00");
    m_pBar = m_Layout.FindPane("B_bar_00");
    m_pAll = m_Layout.FindPane("N_All");
    m_pThumbGroup = m_Layout.FindGroup("G_btn_00");
    m_pUpArrowGroup = m_Layout.FindGroup("G_triT_00");
    m_pDownArrowGroup = m_Layout.FindGroup("G_triB_00");
}

// 0x002997B0
void CommonScrollBar::LeaveMode()
{
    switch (m_Mode) {
    case MODE_PAD:
        m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
        GetHandCursor()->m_Unknown0x4D7 = false;
        break;
    case MODE_TOUCH:
        if (m_IsDragging == true) {
            EndDrag();
        }
        break;
    case MODE_HAND_CURSOR:
        if (m_IsDragging == true) {
            GetHandCursor()->Func1FA0C4(false);
            m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
            EndDrag();
        }
        SetThumbSelected(false);
        m_Control.Reset();
        m_Control.UnselectActive();
        if (m_TouchAnim.m_IsBound) {
            m_TouchAnim.SetFrame(0.0f);
            m_Layout.Animate();
            m_Layout.UnbindAnimation(&m_TouchAnim);
        }
        break;
    default:
        break;
    }
    m_Mode = MODE_NONE;
}

// 0x002998B4
void CommonScrollBar::EndDrag()
{
    m_IsDragging = false;
    if (!GetHandCursor()->m_IsEnabled) {
        SetThumbSelected(false);
    }
    if (m_ScrollRemainder / m_ScrollSpace > 0.5f && m_MaxScroll > m_Scroll) {
        m_Scroll++;
    }
    Scroll(m_Scroll);
}

// 0x00299938
void CommonScrollBar::UpdateThumbBase()
{
    m_ThumbTop = m_BarTop - m_pThumbCenter->m_Size.height * 0.5f;
}

// 0x0029995C
void CommonScrollBar::StartDrag()
{
    m_IsDragging = true;
    SetThumbSelected(true);
}

// 0x00299968
void CommonScrollBar::SetThumbSelected(bool isSelected)
{
    if (m_IsThumbSelected == isSelected) {
        return;
    }
    m_IsThumbSelected = isSelected;
    m_Layout.BindAnimation(&m_SelectAnim, m_pThumbGroup, false);
    if (isSelected) {
        m_SelectAnim.SetFrame(1.0f);
        if (m_Mode == MODE_HAND_CURSOR) {
            SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_CURSOR, GetPaneSoundPan(m_pThumb));
            m_IsSelectSoundPlayed = true;
        }
    } else {
        m_SelectAnim.SetFrame(0.0f);
    }
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&m_SelectAnim, m_pThumbGroup, false);
}

// 0x00299A14 | libgarden [tier A]
void CommonScrollBar::Initialize(ScrollBarDescription&& desc)
{
    m_ArcReader.LoadArc("Layout/common/scrl_bar.arc");
    m_Layout.SetArcResAccReader(&m_ArcReader);
    m_Layout.Build("scrl_bar.bclyt", NULL, 0x2000);
    m_Layout.m_Layer = 130;
    m_SelectAnim.Initialize("scrl_bar_select.bclan", &m_ArcReader);
    m_TouchAnim.Initialize("scrl_bar_touch.bclan", &m_ArcReader);
    m_KindAnim.Initialize("scrl_bar_kind.bclan", &m_ArcReader);
    m_NgAnim.Initialize("scrl_bar_ng.bclan", &m_ArcReader);
    FindPanes();
    m_ScrollSpace = desc.scrollSpace;
    m_ItemNum = desc.scrollSteps2;
    m_Unknown0x754 = desc.unk0;
    m_VisibleNum = desc.scrollSteps1;
    m_OnScroll = desc.onScroll;
    m_OnSelect = reinterpret_cast<void (*)(void*, s32, s32)>(desc.onSelect);
    m_HalfSpace = desc.scrollSpace * 0.5f;
    SetSize(desc.size);
    UpdateMaxScroll();
    m_BarTop = m_Layout.FindPane("N_barT_pos_00")->m_Translate.y;
    m_BarBottom = m_Layout.FindPane("N_barB_pos_00")->m_Translate.y;
    UpdateAllVisible();
    UpdateThumbSize();
    UpdateThumbBase();
    InitializeButtons();
    Scroll(0);
}

// 0x00299D38 | libgarden [tier A]
void CommonScrollBar::Process()
{
    m_ThumbMove = 0.0f;
    if (m_IsTrackTouched == true) {
        m_IsTrackTouched = false;
    }
    if (m_IsPadModeEntered == true) {
        m_IsPadModeEntered = false;
    }
    if (m_IsFrozen == true) {
        return;
    }
    UpdateMode();
    UpdateRepeat();
    UpdatePad();
    if ((m_pAll->m_Flag & nw::lyt::Pane::FLAG_VISIBLE) == 0) {
        return;
    }
    m_Control.Update();
    if (m_Control.IsSelectOkDone()) {
        // an arrow was chosen: it stays selected for the cursor
        const s32 selected = m_Control.GetSelectedIndex();
        m_Control.UnselectCurrent();
        m_Control.Reset();
        if (GetHandCursor()->m_IsEnabled == true) {
            m_Control.SelectNode(selected, true);
        }
    }
    if (!m_Control.HasCurrent()) {
        m_IsSelectSoundPlayed = false;
    } else if (m_Mode == MODE_HAND_CURSOR && !m_IsSelectSoundPlayed) {
        if (m_Control.GetSelectedIndex() == 0) {
            SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_CURSOR, m_UpArrowPan);
        } else {
            SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_CURSOR, m_DownArrowPan);
        }
        m_IsSelectSoundPlayed = true;
    }
    UpdateHandCursor();
    UpdateTouch();
    UpdateDrag();
    if (m_ThumbMove != 0.0f) {
        m_pThumb->m_Translate.y += m_ThumbMove;
        m_pThumb->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
        m_Layout.Animate();
        UpdateContents();
    }
    if (m_IsDragging == true && m_Mode == MODE_HAND_CURSOR) {
        MoveHandCursorToThumb();
    }
}

// 0x00299F40
void CommonScrollBar::SetFrozen(bool isFrozen)
{
    m_IsFrozen = isFrozen;
    if (isFrozen) {
        m_Control.m_Flags |= CONTROL_FLAG_BLOCKED;
    } else {
        m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
    }
}

// 0x00299F5C
void CommonScrollBar::AddContent(ContentNode* node)
{
    m_Contents.PushBack(node);
}

// 0x00299F68
void CommonScrollBar::SetSize(ScrollBarSize size)
{
    m_Size = size;
    m_Layout.BindAnimation(&m_KindAnim, m_Layout.FindGroup("G_bar_00"), false);
    m_Layout.BindAnimation(&m_KindAnim, m_Layout.FindGroup("G_triT_00"), false);
    m_Layout.BindAnimation(&m_KindAnim, m_Layout.FindGroup("G_triB_00"), false);
    m_KindAnim.SetFrame(static_cast<u8>(m_Size));
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_bar_00"), false);
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_triT_00"), false);
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_triB_00"), false);
}

// 0x0029A080 slot 0x10
void CommonScrollBar::OnDragMove(const nn::math::VEC2& pos)
{
    const f32 y = pos.y + m_GrabOffset;
    const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
    MoveThumb(y - thumb.y);
}

// 0x0029A0EC | libgarden [tier A]
CommonScrollBar::CommonScrollBar(void* container)
    : m_Size(ScrollBarSize::LARGE), m_IsFrozen(false), m_IsThumbSelected(false), m_IsDragging(false), m_IsTrackTouched(false),
      m_IsPadEnabled(true), m_IsScrollByPad(false), m_IsBoard(false), m_IsSelectSoundPlayed(false), m_IsPadModeEntered(false),
      m_Unknown0x701(true), m_IsHorizontal(false), m_IsAudioSelectSound(false), m_BarTop(0.0f), m_BarBottom(0.0f), m_ScrollPerUnit(0.0f),
      m_ThumbMove(0.0f), m_ThumbTop(0.0f), m_GrabOffset(0.0f), m_Scroll(0), m_MaxScroll(0), m_ScrollRemainder(0.0f), m_Selected(-1),
      m_HalfSpace(0.0f), m_ThumbPan(0.0f), m_UpArrowPan(0.0f), m_DownArrowPan(0.0f), m_SoundTop(0.0f), m_SoundBottom(0.0f), m_Mode(MODE_NONE),
      m_ScrollSpace(0.0f), m_VisibleNum(0), m_ItemNum(0), m_Unknown0x754(0), m_OnScroll(NULL), m_OnSelect(NULL), m_pContainer(container),
      m_pThumb(NULL), m_pThumbCenter(NULL), m_pThumbTopShadow(NULL), m_pThumbCenterShadow(NULL), m_pThumbBottomShadow(NULL),
      m_pThumbTopBounding(NULL), m_pThumbCenterBounding(NULL), m_pThumbBottomBounding(NULL), m_pBar(NULL), m_pAll(NULL), m_pThumbGroup(NULL),
      m_pUpArrowGroup(NULL), m_pDownArrowGroup(NULL)
{
}

// 0x0029A2C4 slot 0x00 | libgarden
// 0x0029A254 (deleting dtor)
CommonScrollBar::~CommonScrollBar()
{
    // (only the members are destroyed)
}
