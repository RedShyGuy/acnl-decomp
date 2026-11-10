#include "Other/dCatalogBase.h"

#include "Other/dContentNode.h"
#include "Other/dFormatBuffer.h"
#include "Sound/dSoundMgr.h"
#include "script/dRenderGarden.h"
#include "sead/seadNew.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

#include <new>

namespace {
// the states with their names (Japanese, debug): do nothing, entering, exiting, waiting, list fade-out, list fade-in
// 0x0087B05C
const CatalogBase::Info s_StateInfos[6] = {
    {"何もしない", &CatalogBase::Enter_None, &CatalogBase::State_None},
    {"入場処理", &CatalogBase::Enter_In, &CatalogBase::State_In},
    {"退場処理", &CatalogBase::Enter_Out, &CatalogBase::State_Out},
    {"待機処理", &CatalogBase::Enter_Wait, &CatalogBase::State_Wait},
    {"リストフェードアウト", &CatalogBase::Enter_ListFadeOut, &CatalogBase::State_ListFadeOut},
    {"リストフェードイン", &CatalogBase::Enter_ListFadeIn, &CatalogBase::State_ListFadeIn},
};
} // namespace

// 0x001C51E0
bool CatalogBase::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, sead::Heap* heap)
{
    switch (m_SetupStep) {
    case 0:
        // the layout and its animations
        m_Layout.SetArcResAccReader(arcReader);
        g_FormatBuffer.format("%s.bclyt", m_LayoutName);
        m_Layout.Build(g_FormatBuffer.getCString(), NULL, 0xB000);
        m_Layout.m_Layer = 2;
        g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "in");
        m_InAnim.Initialize(g_FormatBuffer.getCString(), arcReader);
        g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "out");
        m_OutAnim.Initialize(g_FormatBuffer.getCString(), arcReader);
        break;
    case 1:
        // the items and a text for each row
        if (!SetupItems()) {
            return false;
        }
        m_pRenders = static_cast<script::RenderGarden*>(::operator new[](m_RowNum * sizeof(script::RenderGarden), heap, 4));
        for (s32 i = 0; i < m_RowNum; i++) {
            new (&m_pRenders[i]) script::RenderGarden(m_TextLength);
            g_FormatBuffer.format("%s_%02d", m_TextBoxName, i);
            m_pRenders[i].SetTextBoxes(m_Layout.FindTextBox(g_FormatBuffer.getCString()), NULL);
        }
        break;
    case 2: {
        // the rows and the scroll bar
        ScrollBarDescription desc;
        desc.size = m_ScrollBarSize;
        desc.onScroll = OnScrollCallback;
        desc.onSelect = reinterpret_cast<ScrollBarDescription::Callback>(OnSelectCallback);
        m_pFrame = m_Layout.FindPane("N_frame_00");
        if (m_pFrame == NULL) {
            m_pFrame = m_Layout.FindPane("N_Frame_Pos");
        }
        InitializeContents(arcReader);
        m_RowSpace = m_pContents[0].m_BasePosition.y - m_pContents[1].m_BasePosition.y;
        desc.scrollSpace = m_RowSpace;
        // the frame is one row higher, the layout one row lower
        m_pFrame->SetTranslate(nn::math::VEC3(m_pFrame->m_Translate.x, m_pFrame->m_Translate.y + m_RowSpace, m_pFrame->m_Translate.z));
        UpdateScissor();
        nw::lyt::Pane* root = m_Layout.GetRootPane();
        root->SetTranslate(nn::math::VEC3(root->m_Translate.x, root->m_Translate.y - m_RowSpace, root->m_Translate.z));
        desc.scrollSteps1 = m_RowNum - 1;
        desc.scrollSteps2 = m_ItemNum;
        desc.unk0 = m_ItemNum;
        m_ScrollBar.Initialize(static_cast<ScrollBarDescription&&>(desc));
        m_ScrollBar.Scroll(0);
        if (m_ItemNum < m_RowNum - 1) {
            m_ScrollBar.m_pAll->SetVisible(false);
        }
        break;
    }
    default:
        m_ScrollBar.m_Selected = -1;
        Start("CatalogBase", s_StateInfos, 6);
        return true;
    }
    m_SetupStep++;
    return false;
}

// 0x001C5590
void CatalogBase::OnSelectCallback(void* container, s32 selected, s32 prev)
{
    static_cast<CatalogBase*>(container)->OnSelect(selected, prev);
}

// 0x001C559C slot 0x40
void CatalogBase::OnSelect(s32 selected, s32 prev)
{
    // empty (in the original too)
}

// 0x001C55A0
void CatalogBase::Reset()
{
    UpdateItemNum();
    m_ScrollBar.SetUnknown0x754(m_ItemNum);
    m_ScrollBar.SetItemNum(m_ItemNum, false);
    m_ScrollBar.m_pAll->SetVisible(m_RowNum - 1 <= m_ItemNum);
    InitializeScroll();
    ApplyScroll();
    if (m_ScrollBar.m_ThumbMove != 0.0f) {
        m_Layout.m_IsAnimationChanged = true;
    }
}

// 0x001C564C
void CatalogBase::State_In()
{
    UpdateIn();
}

// 0x001C5658
void CatalogBase::Enter_In()
{
    m_Layout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_Layout.m_IsAnimationChanged = true;
    m_IsAnimating = true;
    OnEnterIn();
}

// 0x001C569C
void CatalogBase::UpdateScissor()
{
    const nw::lyt::Pane* frame = m_pFrame;
    const f32 width = frame->m_Size.width;
    const f32 height = frame->m_Size.height;
    const f32 right = frame->m_GlbMtx.m[0][3] + width * 0.5f;
    const f32 bottom = frame->m_GlbMtx.m[1][3] - height * 0.5f;
    // from the layout (center 0, y up) to the lower screen (320x240, y down)
    m_Layout.m_IsScissorEnable = true;
    m_Layout.m_ScissorX = 160.0f - right;
    m_Layout.m_ScissorY = bottom + 120.0f;
    m_Layout.m_ScissorWidth = width;
    m_Layout.m_ScissorHeight = height;
}

// 0x001C56F8
void CatalogBase::State_Out()
{
    UpdateOut();
}

// 0x001C5704
void CatalogBase::Enter_Out()
{
    m_Layout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_Layout.m_IsAnimationChanged = true;
    m_IsAnimating = true;
    m_IsScrollBarStopped = true;
    OnEnterOut();
}

// 0x001C574C
void CatalogBase::State_None()
{
    // empty (in the original too)
}

// 0x001C5750
void CatalogBase::State_Wait()
{
    UpdateWait();
}

// 0x001C575C
void CatalogBase::Enter_None()
{
    // empty (in the original too)
}

// 0x001C5760
void CatalogBase::Enter_Wait()
{
    OnEnterWait();
}

// 0x001C576C
void CatalogBase::State_ListFadeIn()
{
    if (m_ListInAnim.IsDone()) {
        m_Layout.UnbindAnimation(&m_ListInAnim);
        m_IsListFading = false;
        m_IsScrollBarStopped = false;
        Change(&CatalogBase::State_Wait);
        return;
    }
    m_ListInAnim.IncreaseFrame();
    m_Layout.m_IsAnimationChanged = true;
}

// 0x001C57C8
void CatalogBase::Enter_ListFadeIn()
{
    m_Layout.BindAnimation(&m_ListInAnim);
    m_ListInAnim.SetFrame(0.0f);
    m_Layout.m_IsAnimationChanged = true;
    m_IsListFading = true;
}

// 0x001C57FC
void CatalogBase::State_ListFadeOut()
{
    if (m_ListOutAnim.IsDone()) {
        // the list is invisible: the items change
        m_Layout.UnbindAnimation(&m_ListOutAnim);
        UpdateItemNum();
        m_ScrollBar.SetUnknown0x754(m_ItemNum);
        m_ScrollBar.SetItemNum(m_ItemNum, false);
        m_ScrollBar.m_pAll->SetVisible(m_RowNum - 1 <= m_ItemNum);
        InitializeScroll();
        ApplyScroll();
        if (m_ScrollBar.m_ThumbMove != 0.0f) {
            m_Layout.m_IsAnimationChanged = true;
        }
        m_IsListFading = false;
        Change(m_ListFadeNext);
        return;
    }
    m_ListOutAnim.IncreaseFrame();
    m_Layout.m_IsAnimationChanged = true;
}

// 0x001C58E8
void CatalogBase::Enter_ListFadeOut()
{
    m_IsListFading = true;
    m_Layout.BindAnimation(&m_ListOutAnim);
    m_ListOutAnim.SetFrame(0.0f);
    m_Layout.m_IsAnimationChanged = true;
    if (m_IsListFadeSound) {
        SoundMgr::PlaySound(SE_SYS_CTLG_PAGE_CHANGE, 0.0f);
    }
    m_IsScrollBarStopped = true;
}

// 0x001C5948 slot 0x44
void CatalogBase::UpdateItemNum()
{
    // empty (in the original too)
}

// 0x001C5974 slot 0x14
void CatalogBase::Draw()
{
    m_Layout.Animate();
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Layout, true);
}

// 0x001C5998
void CatalogBase::StartIn()
{
    Change(&CatalogBase::State_In);
}

// 0x001C59B0 slot 0x10
void CatalogBase::Update()
{
    if (!m_IsScrollBarStopped) {
        m_ScrollBar.Process();
        if (m_ScrollBar.m_ThumbMove != 0.0f) {
            m_Layout.m_IsAnimationChanged = true;
        }
    }
    Call();
}

// 0x001C5A6C slot 0x50
void CatalogBase::UpdateIn()
{
    if (m_InAnim.IsDone()) {
        m_Layout.UnbindAnimation(&m_InAnim);
        m_IsAnimating = false;
        m_IsScrollBarStopped = false;
        Change(&CatalogBase::State_Wait);
        return;
    }
    m_InAnim.IncreaseFrame();
    m_Layout.Animate();
    UpdateScissor();
}

// 0x001C5AD4 slot 0x4C
void CatalogBase::OnEnterIn()
{
    // empty (in the original too)
}

// 0x001C5AD8
void CatalogBase::StartOut()
{
    Change(&CatalogBase::State_Out);
}

// 0x001C5AF0
void CatalogBase::OnScrollCallback(void* container)
{
    static_cast<CatalogBase*>(container)->OnScroll();
}

// 0x001C5AFC slot 0x58
void CatalogBase::UpdateOut()
{
    if (m_OutAnim.IsDone()) {
        m_Layout.UnbindAnimation(&m_OutAnim);
        m_IsAnimating = false;
        Change(&CatalogBase::State_None);
        return;
    }
    m_OutAnim.IncreaseFrame();
    m_Layout.Animate();
    UpdateScissor();
}

// 0x001C5B60 slot 0x54
void CatalogBase::OnEnterOut()
{
    // empty (in the original too)
}

// 0x001C5B64 slot 0x60
void CatalogBase::UpdateWait()
{
    // empty (in the original too)
}

// 0x001C5B68 slot 0x5C
void CatalogBase::OnEnterWait()
{
    // empty (in the original too)
}

// 0x001C5B6C
CatalogBase::CatalogBase()
    : m_LayoutName(NULL), m_TextBoxName("T_itm"), m_pFrame(NULL), m_pRenders(NULL), m_pContents(NULL), m_ScrollBar(this),
      m_ScrollBarSize(ScrollBarSize::SMALL), m_RowNum(-1), m_ItemNum(0), m_TextLength(0), m_RowSpace(0.0f), m_IsAnimating(false),
      m_IsScrollBarStopped(true), m_IsListFading(false), m_IsListFadeSound(true), m_ListFadeNext(&CatalogBase::State_ListFadeIn),
      m_SetupStep(0)
{
}

// 0x001C5CF4 slot 0x00
// 0x001C5C50 (deleting dtor)
CatalogBase::~CatalogBase()
{
    // (only the members are destroyed)
}

// 0x002988A0 slot 0x38 (falls into CommonScrollBar::Scroll)
void CatalogBase::InitializeScroll()
{
    m_ScrollBar.Scroll(0);
}

// 0x00299F00 slot 0x48
void CatalogBase::ApplyScroll()
{
    // one frame of the scroll bar without the cursor keys and without input
    m_ScrollBar.m_IsPadEnabled = false;
    m_ScrollBar.SetFrozen(true);
    m_ScrollBar.Process();
    m_ScrollBar.m_IsPadEnabled = true;
    m_ScrollBar.SetFrozen(false);
}

// 0x002F88F4 slot 0x0C
void CatalogBase::Finalize()
{
    m_Layout.UnbindAllAnimation();
    m_ScrollBar.Finalize();
    for (s32 i = 0; i < m_RowNum; i++) {
        m_pRenders[i].Clear();
        m_pRenders[i].~RenderGarden();
    }
    ::operator delete[](m_pRenders);
}

// 0x0082AA40 slot 0x08
const char* CatalogBase::GetName(Func func) const
{
    return FindName(func);
}

// 0x00817C18 (name is ours)
template void state::Mode<CatalogBase>::Change(Func func);
