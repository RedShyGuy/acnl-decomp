#include "Catalog/dFtrList.h"

#include "Other/dPaneVisible.h"
#include "Item/item_Api.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

// 0x0095081C
const char* FtrList::s_TabPaneNames[TAB_NUM] = {"P_leaf_00", "P_wall_00", "P_flr_00", "P_takumi_00"};

// 0x007F4DEC
template CatalogBase* CatalogBase::Create<FtrList>();

// 0x0060D79C
FtrList::FtrList() : m_ShownTab(0)
{
    m_TextLength = 33;
    m_ScrollBar.m_Unknown0x701 = false;
}

// 0x0060DA4C slot 0x00
// 0x0060D918 (deleting dtor)
FtrList::~FtrList()
{
    // (only the members and the base are destroyed)
}

// 0x0060D280 slot 0x0C
void FtrList::Finalize()
{
    for (s32 i = 0; i < TAB_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "N_pos", i);
        m_Tab.m_Layout.FindPane(g_FormatBuffer.getCString())->RemoveChild(m_Tab.m_TabLayouts[i].GetRootPane());
    }
    SelectBase::Finalize();
}

// 0x0060D2FC slot 0x10
void FtrList::Update()
{
    if (!m_IsListFading) {
        m_Tab.Update(m_IsScrollBarStopped);
        // the list waits while a tab is chosen and until the items of the new tab are shown
        m_Control.SetFlags(ButtonActionControl::DISABLED,
                           m_IsScrollBarStopped || m_Tab.IsBusy() || m_Tab.m_Current != m_ShownTab);
        m_Control.Update();
        if (!m_IsScrollBarStopped && !m_Tab.IsBusy() && !(m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing())) {
            m_ScrollBar.Process();
            if (m_ScrollBar.m_ThumbMove != 0.0f) {
                m_Layout.m_IsAnimationChanged = true;
            }
            if (m_ScrollBar.m_IsPadModeEntered == true) {
                m_Control.UnselectCurrent();
                m_Tab.m_Control.UnselectCurrent();
            }
        }
    }
    Call();
}

// 0x0060D254 slot 0x14
void FtrList::Draw()
{
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Tab.m_Layout, true);
    CatalogBase::Draw();
}

// 0x0060D114 slot 0x30
void FtrList::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    SelectBase::InitializeContents(arcReader);
    m_Tab.Initialize(arcReader);
    // the icons of the tabs
    for (s32 i = 0; i < TAB_NUM; i++) {
        const char* paneName = s_TabPaneNames[i];
        m_Tab.m_TabLayouts[i].SetGroupVisible("G_tab_00", false);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane("N_itm_scrn_00"), true);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane("P_tab_00"), true);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane(paneName), true);
    }
    m_ShownTab = 0;
}

// 0x0060D5C4 slot 0x34
bool FtrList::SetupItems()
{
    m_pNames[1] = m_WallNames;
    m_pIds[1] = m_WallIds;
    m_ItemNums[1] = CollectIds(m_WallIds, item::KIND_DUMMY_WALL, false);
    m_pNames[2] = m_FloorNames;
    m_pIds[2] = m_FloorIds;
    m_ItemNums[2] = CollectIds(m_FloorIds, item::KIND_CARPET, false);
    m_pNames[0] = m_LeafNames;
    m_pIds[0] = m_LeafIds;
    m_ItemNums[0] = CollectIds(m_LeafIds, item::KIND_DUMMY_FTR, false);
    m_pNames[3] = m_TakumiNames;
    m_pIds[3] = m_TakumiIds;
    m_ItemNums[3] = CollectIds(m_TakumiIds, item::KIND_DUMMY_FTR, true);
    for (u32 i = 0; i < TAB_NUM; i++) {
        SortIds(m_pIds[i], m_ItemNums[i]);
        SetNames(m_pNames[i], m_pIds[i], m_ItemNums[i]);
    }
    m_ItemNum = m_ItemNums[1];
    return true;
}

// 0x0060D230 slot 0x38
void FtrList::InitializeScroll()
{
    m_ScrollBar.Scroll(m_Tab.m_Scroll[m_Tab.m_Current]);
}

// 0x0060D1E8 slot 0x44
void FtrList::UpdateItemNum()
{
    const s32 tab = m_Tab.m_Current;
    m_Selected = m_Tab.m_Selected[tab];
    m_ShownTab = tab;
    m_ItemNum = m_ItemNums[tab];
}

// 0x0060D578 slot 0x4C
void FtrList::OnEnterIn()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_In);
    Reset();
}

// 0x0060D5A4 slot 0x54
void FtrList::OnEnterOut()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_Out);
}

// 0x0060D730 slot 0x60
void FtrList::UpdateWait()
{
    SelectBase::UpdateWait();
    // another tab was chosen: the list keeps the selection and the scroll of this one and fades to the other
    if (m_Tab.m_Current != m_ShownTab) {
        m_Tab.m_Selected[m_ShownTab] = m_Selected;
        m_Tab.m_Scroll[m_ShownTab] = m_ScrollBar.m_Scroll;
        Change(&CatalogBase::State_ListFadeOut);
    }
}
