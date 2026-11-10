#include "Catalog/dExcavateList.h"

#include "Other/dPaneVisible.h"
#include "Item/item_Api.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

// 0x00950804
const char* ExcavateList::s_TabPaneNames[TAB_NUM] = {"P_clay_00", "P_fsl_00"};

// 0x00200A10 (falls into ExcavateList::ExcavateList)
template CatalogBase* CatalogBase::Create<ExcavateList>();

// 0x00200A30
ExcavateList::ExcavateList() : m_ShownTab(0), m_SetupStep(0)
{
    m_TextLength = 33;
    m_ScrollBar.m_Unknown0x701 = false;
}

// 0x00200C38 slot 0x00
// 0x00200B50 (deleting dtor)
ExcavateList::~ExcavateList()
{
    // (only the members and the base are destroyed)
}

// 0x0020054C slot 0x0C
void ExcavateList::Finalize()
{
    for (s32 i = 0; i < TAB_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "N_pos", i);
        m_Tab.m_Layout.FindPane(g_FormatBuffer.getCString())->RemoveChild(m_Tab.m_TabLayouts[i].GetRootPane());
    }
    SelectBase::Finalize();
}

// 0x002005D4 slot 0x10
void ExcavateList::Update()
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

// 0x00200520 slot 0x14
void ExcavateList::Draw()
{
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Tab.m_Layout, true);
    CatalogBase::Draw();
}

// 0x00200038 slot 0x30
void ExcavateList::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
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

// 0x002008A8 slot 0x34
bool ExcavateList::SetupItems()
{
    if (m_SetupStep == 0) {
        m_pNames[0] = m_HaniwaNames;
        m_pIds[0] = m_HaniwaIds;
        m_ItemNums[0] = CollectIds(m_HaniwaIds, item::KIND_HANIWA, false);
        m_pNames[1] = m_FossilNames;
        m_pIds[1] = m_FossilIds;
        m_ItemNums[1] = CollectIds(m_FossilIds, item::KIND_FOSSIL, false);
        m_SetupStep++;
        return false;
    }
    for (u32 i = 0; i < TAB_NUM; i++) {
        SortIds(m_pIds[i], m_ItemNums[i]);
        SetNames(m_pNames[i], m_pIds[i], m_ItemNums[i]);
    }
    m_ItemNum = m_ItemNums[0];
    return true;
}

// 0x002004FC slot 0x38
void ExcavateList::InitializeScroll()
{
    m_ScrollBar.Scroll(m_Tab.m_Scroll[m_Tab.m_Current]);
}

// 0x002004BC slot 0x44
void ExcavateList::UpdateItemNum()
{
    const s32 tab = m_Tab.m_Current;
    m_Selected = m_Tab.m_Selected[tab];
    m_ShownTab = tab;
    m_ItemNum = m_ItemNums[tab];
}

// 0x0020085C slot 0x4C
void ExcavateList::OnEnterIn()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_In);
    Reset();
}

// 0x00200888 slot 0x54
void ExcavateList::OnEnterOut()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_Out);
}

// 0x002009AC slot 0x60
void ExcavateList::UpdateWait()
{
    SelectBase::UpdateWait();
    // another tab was chosen: the list keeps the selection and the scroll of this one and fades to the other
    if (m_Tab.m_Current != m_ShownTab) {
        m_Tab.m_Selected[m_ShownTab] = m_Selected;
        m_Tab.m_Scroll[m_ShownTab] = m_ScrollBar.m_Scroll;
        Change(&CatalogBase::State_ListFadeOut);
    }
}
