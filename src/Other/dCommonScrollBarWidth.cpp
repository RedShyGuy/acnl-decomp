#include "Other/dCommonScrollBarWidth.h"

#include "Other/dContentNode.h"
#include "Other/dController.h"
#include "Other/dMenuBase.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "font/dMgr.h"
#include "nw/lyt/lyt_Pane.h"

namespace {
// the flag of ButtonActionControl::m_Flags the scroll bar sets while it has the input
const u32 CONTROL_FLAG_BLOCKED = 1 << 3;
} // namespace

// 0x0031D470 slot 0x18
void CommonScrollBarWidth::UpdateContents()
{
    UpdateScrollFromThumb();
    for (ContentNode* node = static_cast<ContentNode*>(m_Contents.GetFirst()); node != NULL;
         node = static_cast<ContentNode*>(node->GetNext())) {
        nw::lyt::Pane* pane = node->m_pPane;
        pane->SetTranslate(nn::math::VEC3(node->m_BasePosition.x - m_ScrollRemainder, pane->m_Translate.y, pane->m_Translate.z));
    }
}

// 0x0031D4DC slot 0x0C
void CommonScrollBarWidth::UpdateTouch()
{
    if (m_Mode != MODE_TOUCH) {
        return;
    }
    UpdateArrowButtons();
    if (Controller::IsTrig(Controller::TOUCH)) {
        nn::math::VEC2 pos;
        m_Layout.ScreenToLayout(reinterpret_cast<const nn::math::VEC2&>(Controller::GetPointer()), &pos);
        if (IsOnThumb(pos)) {
            // the thumb is grabbed
            StartDrag();
            const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
            m_GrabOffset = thumb.x - pos.x;
            SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_ACTIVE, GetPaneSoundPan(m_pThumb));
        } else if (IsOnBar(pos)) {
            OnTouchTrack(pos);
        }
    } else if (Controller::IsRelease(Controller::TOUCH) && m_IsDragging == true) {
        EndDrag();
    }
}

// 0x0031D5E8 slot 0x14
void CommonScrollBarWidth::MoveThumb(f32 dx)
{
    // a move to the right is a move towards the start of the bar
    m_ThumbMove = -dx;
    ClampThumbMove();
}

// 0x0031D5F8
void CommonScrollBarWidth::UpdateButtons()
{
    m_Control.Update();
    if (m_Control.IsSelectOkDone()) {
        m_Control.UnselectCurrent();
        m_Control.Reset();
    }
}

// 0x0031D634
void CommonScrollBarWidth::InitializeButtons()
{
    // copied from the image 0x00858CC8 (the descriptions 0x00858CD8/0x00858CFC)
    const char* const arrowPanes[2] = {"B_triT_00", "B_triB_00"};
    const char* const arrowGroups[2] = {"G_triT_00", "G_triB_00"};
    ButtonActionControl::Description desc;
    if (m_IsBoard == true) {
        const ButtonActionControl::Description boardDesc = {
            &m_ArcReader,
            &m_Layout,
            NULL,
            "board_scrl_bar_touch.bclan",
            "board_scrl_bar_select.bclan",
            "board_scrl_bar_touch_ok.bclan",
            "board_scrl_bar_select_ok.bclan",
            3,
            NULL,
        };
        desc = boardDesc;
    } else {
        const ButtonActionControl::Description widthDesc = {
            &m_ArcReader,
            &m_Layout,
            NULL,
            "scrl_bar_W_touch.bclan",
            "scrl_bar_W_select.bclan",
            "scrl_bar_W_touch_ok.bclan",
            "scrl_bar_W_select_ok.bclan",
            3,
            NULL,
        };
        desc = widthDesc;
    }
    m_Control.Initialize(desc, NULL);
    m_Control.m_Flags |= 0x40;
    ButtonActionNode::Info info;
    info.pane = NULL;
    info.group = NULL;
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.input = 0;
    info.index = 0;
    info.cursorCallback = NULL;
    const u32 inputs[2] = {0, 0};
    for (s32 i = 0; i < 2; i++) {
        info.pane = m_Layout.FindPane(arrowPanes[i]);
        info.group = m_Layout.FindGroup(arrowGroups[i]);
        info.input = inputs[i];
        info.index = i;
        ButtonActionNode& arrow = m_Arrows[i];
        arrow.Initialize(info);
        arrow.m_OkSoundEnabled = false;
        arrow.m_SoundEnabled = false;
        m_Control.AddNode(&arrow, true);
    }
    m_Control.SetCursor(false);
    m_Control.m_Flags &= ~CONTROL_FLAG_BLOCKED;
    m_Control.m_Flags2 = 35;
}

// 0x0031D7D0 slot 0x08
void CommonScrollBarWidth::OnTouchTrack(const nn::math::VEC2& pos)
{
    m_IsTrackTouched = true;
    SoundMgr::PlaySound(SE_SYS_SCROLL_BAR_COME, m_ThumbPan);
    const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
    MoveThumb(pos.x - thumb.x);
    StartDrag();
    m_GrabOffset = 0.0f;
}

// 0x0031D844
void CommonScrollBarWidth::Initialize(ScrollBarDescription&& desc, bool isBoard)
{
    m_ArcReader.LoadArc("Layout/common/scrl_bar.arc");
    font::Mgr* fonts = font::Mgr::s_pInstance;
    m_ArcReader.GetResourceAccessorVRAM()->RegistFont(fonts->GetFontName(font::GARDEN_MSG_16)->getCString(),
                                                      fonts->GetFont(font::GARDEN_MSG_16));
    m_Layout.SetArcResAccReader(&m_ArcReader);
    m_IsBoard = isBoard;
    if (isBoard) {
        m_Layout.Build("board_scrl_bar.bclyt", NULL, 0x2000);
        m_Layout.m_Layer = 130;
        m_SelectAnim.Initialize("board_scrl_bar_select.bclan", &m_ArcReader);
        m_TouchAnim.Initialize("board_scrl_bar_touch.bclan", &m_ArcReader);
        m_KindAnim.Initialize("board_scrl_bar_kind.bclan", &m_ArcReader);
        m_NgAnim.Initialize("board_scrl_bar_ng.bclan", &m_ArcReader);
    } else {
        m_Layout.Build("scrl_bar_W.bclyt", NULL, 0x2000);
        m_Layout.m_Layer = 130;
        m_SelectAnim.Initialize("scrl_bar_W_select.bclan", &m_ArcReader);
        m_TouchAnim.Initialize("scrl_bar_W_touch.bclan", &m_ArcReader);
        m_KindAnim.Initialize("scrl_bar_W_kind.bclan", &m_ArcReader);
        m_NgAnim.Initialize("scrl_bar_W_ng.bclan", &m_ArcReader);
    }
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
    // (the panes of the bar are turned in the layout: along the bar it is still y)
    m_BarTop = m_Layout.FindPane("N_barT_pos_00")->m_Translate.y;
    m_BarBottom = m_Layout.FindPane("N_barB_pos_00")->m_Translate.y;
    UpdateAllVisible();
    UpdateThumbSize();
    UpdateThumbBase();
    InitializeButtons();
    Scroll(0);
    m_IsHorizontal = true;
}

// 0x0031DB6C slot 0x10
void CommonScrollBarWidth::OnDragMove(const nn::math::VEC2& pos)
{
    const f32 x = pos.x + m_GrabOffset;
    const nn::math::VEC3 thumb = GetGlobalPosition(m_pThumb);
    MoveThumb(x - thumb.x);
}

// 0x0031DBC0
CommonScrollBarWidth::CommonScrollBarWidth(void* container) : CommonScrollBar(container)
{
}

// 0x0031DC48 slot 0x00
// 0x0031DBD8 (deleting dtor)
CommonScrollBarWidth::~CommonScrollBarWidth()
{
    // (only the members are destroyed)
}
