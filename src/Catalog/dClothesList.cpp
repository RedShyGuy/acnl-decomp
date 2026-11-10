#include "Catalog/dClothesList.h"

#include "Other/dPaneVisible.h"
#include "Item/item_Api.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

// 0x0095082C
const char* ClothesList::s_TabPaneNames[TAB_NUM] = {"P_top_00", "P_btm_00", "P_drs_00", "P_cap_00",
                                                     "P_acs_00", "P_shoe_00", "P_umb_00"};

// 0x007F4D2C
template CatalogBase* CatalogBase::Create<ClothesList>();

// 0x001C7AE8
ClothesList::ClothesList() : m_ShownTab(0), m_SetupStep(0)
{
    m_TextLength = 33;
    m_ScrollBar.m_Unknown0x701 = false;
}

// 0x001C7DFC slot 0x00
// 0x001C7C90 (deleting dtor)
ClothesList::~ClothesList()
{
    // (only the members and the base are destroyed)
}

// 0x001C7474 slot 0x0C
void ClothesList::Finalize()
{
    for (s32 i = 0; i < TAB_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "N_pos", i);
        m_Tab.m_Layout.FindPane(g_FormatBuffer.getCString())->RemoveChild(m_Tab.m_TabLayouts[i].GetRootPane());
    }
    SelectBase::Finalize();
}

// 0x001C74FC slot 0x10
void ClothesList::Update()
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

// 0x001C7448 slot 0x14
void ClothesList::Draw()
{
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Tab.m_Layout, true);
    CatalogBase::Draw();
}

// 0x001C6F48 slot 0x30
void ClothesList::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
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

// 0x001C77CC slot 0x34
bool ClothesList::SetupItems()
{
    switch (m_SetupStep) {
    case 0:
        m_pNames[0] = m_TopsNames;
        m_pIds[0] = m_TopsIds;
        m_ItemNums[0] = CollectIds(m_TopsIds, item::KIND_TOPS, false);
        m_pNames[1] = m_BottomsNames;
        m_pIds[1] = m_BottomsIds;
        m_ItemNums[1] = CollectIds(m_BottomsIds, item::KIND_BOTTOMS, false);
        m_pNames[2] = m_OnePieceNames;
        m_pIds[2] = m_OnePieceIds;
        m_ItemNums[2] = CollectIds(m_OnePieceIds, item::KIND_ONE_PIECE_OUTFIT, false);
        m_pNames[3] = m_CapNames;
        m_pIds[3] = m_CapIds;
        m_ItemNums[3] = CollectIds(m_CapIds, item::KIND_CAP, false);
        break;
    case 1:
        m_pNames[4] = m_AcceNames;
        m_pIds[4] = m_AcceIds;
        m_ItemNums[4] = CollectIds(m_AcceIds, item::KIND_ACCE, false);
        // the socks follow the shoes
        m_pNames[5] = m_ShoesNames;
        m_pIds[5] = m_ShoesIds;
        m_ItemNums[5] = CollectIds(m_ShoesIds, item::KIND_SHOES, false);
        m_pNames[7] = &m_ShoesNames[m_ItemNums[5]];
        m_pIds[7] = &m_ShoesIds[m_ItemNums[5]];
        m_ItemNums[5] += CollectIds(&m_ShoesIds[m_ItemNums[5]], item::KIND_SOCKS, false);
        m_pNames[6] = m_UmbrellaNames;
        m_pIds[6] = m_UmbrellaIds;
        m_ItemNums[6] = CollectIds(m_UmbrellaIds, item::KIND_UMBRELLA, false);
        break;
    case 2:
        for (u32 i = 0; i < TAB_NUM; i++) {
            SortIds(m_pIds[i], m_ItemNums[i]);
            SetNames(m_pNames[i], m_pIds[i], m_ItemNums[i]);
        }
        m_ItemNum = m_ItemNums[0];
        return true;
    default:
        break;
    }
    m_SetupStep++;
    return false;
}

// 0x001C7420 slot 0x38
void ClothesList::InitializeScroll()
{
    m_ScrollBar.Scroll(m_Tab.m_Scroll[m_Tab.m_Current]);
}

// 0x001C73D8 slot 0x44
void ClothesList::UpdateItemNum()
{
    const s32 tab = m_Tab.m_Current;
    m_Selected = m_Tab.m_Selected[tab];
    m_ShownTab = tab;
    m_ItemNum = m_ItemNums[tab];
}

// 0x001C7780 slot 0x4C
void ClothesList::OnEnterIn()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_In);
    Reset();
}

// 0x001C77AC slot 0x54
void ClothesList::OnEnterOut()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_Out);
}

// 0x001C7A7C slot 0x60
void ClothesList::UpdateWait()
{
    SelectBase::UpdateWait();
    // another tab was chosen: the list keeps the selection and the scroll of this one and fades to the other
    if (m_Tab.m_Current != m_ShownTab) {
        m_Tab.m_Selected[m_ShownTab] = m_Selected;
        m_Tab.m_Scroll[m_ShownTab] = m_ScrollBar.m_Scroll;
        Change(&CatalogBase::State_ListFadeOut);
    }
}
