#include "Other/dCatalogList.h"

#include "Other/dPaneVisible.h"
#include <new>

#include "Item/item_Api.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dRenderGarden.h"
#include "script/dScriptExtern.h"
#include "script/dStrMgr.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

// 0x009507F8
const char* CatalogList::s_ListPaneName = "N_list";
// 0x0095080C
const char* CatalogList::s_TabPaneNames[TAB_NUM] = {"P_fsl_00", "P_bug_00", "P_fish_00", "P_draw_00"};

// 0x007F4D08
template CatalogBase* CatalogBase::Create<CatalogList>();

// 0x001C6ACC
CatalogList::CatalogList() : m_pDonorRenders(NULL), m_ShownTab(0), m_DonorLength(9)
{
    m_TextLength = 33;
    m_LayoutName = "ctlg_cntnt_01";
}

// 0x001C6D04 slot 0x00
// 0x001C6CF4 (deleting dtor)
CatalogList::~CatalogList()
{
    // (only the members and the base are destroyed)
}

// 0x001C6228 slot 0x0C
void CatalogList::Finalize()
{
    for (s32 i = 0; i < TAB_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "N_pos", i);
        m_Tab.m_Layout.FindPane(g_FormatBuffer.getCString())->RemoveChild(m_Tab.m_TabLayouts[i].GetRootPane());
    }
    for (s32 i = 0; i < m_RowNum; i++) {
        m_pDonorRenders[i].Clear();
        m_pDonorRenders[i].~RenderGarden();
    }
    ssys::ma::lyt::GetAllocator()->Free(m_pDonorRenders);
    CatalogBase::Finalize();
}

// 0x001C6300 slot 0x10
void CatalogList::Update()
{
    if (!m_IsListFading) {
        m_Tab.Update(m_IsScrollBarStopped);
        if (!m_IsScrollBarStopped && !m_Tab.IsBusy()) {
            m_ScrollBar.Process();
            if (m_ScrollBar.m_ThumbMove != 0.0f) {
                m_Layout.m_IsAnimationChanged = true;
            }
            if (m_ScrollBar.m_IsPadModeEntered == true) {
                m_Tab.m_Control.UnselectCurrent();
            }
        }
    }
    Call();
    m_Layout.Animate();
}

// 0x001C594C slot 0x14 (falls into CatalogBase::Draw)
void CatalogList::Draw()
{
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Tab.m_Layout, true);
    CatalogBase::Draw();
}

// 0x001C5D94 slot 0x30
void CatalogList::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "fade_out");
    m_ListOutAnim.Initialize(g_FormatBuffer.getCString(), arcReader);
    g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "fade_in");
    m_ListInAnim.Initialize(g_FormatBuffer.getCString(), arcReader);
    // the rows and their bases scroll
    for (s32 i = 0; i < m_RowNum; i++) {
        g_FormatBuffer.format("%s_%02d", s_ListPaneName, i);
        nw::lyt::Pane* pane = m_Layout.FindPane(g_FormatBuffer.getCString());
        m_pContents[i].m_pPane = pane;
        m_pContents[i].m_BasePosition = pane->m_Translate;
        m_pContents[i].m_BasePosition = m_pContents[i].m_pPane->m_Translate;
        m_ScrollBar.AddContent(&m_pContents[i]);
        g_FormatBuffer.format("%s_%02d", "N_list_base", i);
        nw::lyt::Pane* base = m_Layout.FindPane(g_FormatBuffer.getCString());
        m_BaseContents[i].m_pPane = base;
        m_BaseContents[i].m_BasePosition = base->m_Translate;
        m_BaseContents[i].m_BasePosition = base->m_Translate;
        m_ScrollBar.AddContent(&m_BaseContents[i]);
    }
    m_ScrollBar.m_IsScrollByPad = true;
    m_Tab.Initialize(arcReader);
    // the icons of the tabs
    for (s32 i = 0; i < TAB_NUM; i++) {
        const char* paneName = s_TabPaneNames[i];
        m_Tab.m_TabLayouts[i].SetGroupVisible("G_tab_00", false);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane("N_itm_scrn_00"), true);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane("P_tab_00"), true);
        SetFlagVisible(m_Tab.m_TabLayouts[i].FindPane(paneName), true);
    }
}

// 0x001C6528
s32 CatalogList::CollectPart(s32 part, u8 kind)
{
    script::WordFix<10>* donors = m_pDonors[part];
    script::WordFix<33>* names = m_pNames[part];
    u16* ids = m_pIds[part];
    s32 num = 0;
    const ItemNameRange* range = Func5360A8(kind);
    SvMuseum* museum = &GetGardenPlus()->data.townData.museum;
    // (the items that no player of the town gave)
    script::WordFix<9> noDonor;
    script::StrMgr::s_pInstance->GetString(&noDonor, "STR_Common", 40);
    for (s32 i = range->first; i <= range->last; i++) {
        *ids = i;
        const SvFgName name(static_cast<SvFgName::ID>(i));
        if (Func76D8A8(museum, &name)) {
            num++;
            item::GetName(names, name, NULL);
            SvPersonalId donor;
            if (Func76D974(museum, &donor, &name)) {
                script::WordFix<9> donorName;
                MakePlayerName(&donorName, donor.playerData.playerName, donor.playerData.gender & 1);
                donors->Copy(donorName);
            } else {
                donors->Copy(noDonor);
            }
            names++;
            ids++;
            donors++;
        }
    }
    return num;
}

// 0x001C6784 slot 0x34
bool CatalogList::SetupItems()
{
    m_DonorLength = 9;
    m_pNames[0] = m_FossilNames;
    m_pIds[0] = m_FossilIds;
    m_pDonors[0] = m_FossilDonors;
    m_ItemNums[0] = CollectPart(0, item::KIND_FOSSIL);
    m_pNames[1] = m_InsectNames;
    m_pIds[1] = m_InsectIds;
    m_pDonors[1] = m_InsectDonors;
    m_ItemNums[1] = CollectPart(1, item::KIND_INSECT);
    // the sea creatures follow the fish, the sculptures the paintings
    m_pNames[2] = m_FishNames;
    m_pIds[2] = m_FishIds;
    m_pDonors[2] = m_FishDonors;
    m_ItemNums[2] = CollectPart(2, item::KIND_FISH);
    m_pNames[4] = &m_FishNames[m_ItemNums[2]];
    m_pIds[4] = &m_FishIds[m_ItemNums[2]];
    m_pDonors[4] = &m_FishDonors[m_ItemNums[2]];
    m_ItemNums[2] += CollectPart(4, item::KIND_DIVE_FISH);
    m_pNames[3] = m_ArtNames;
    m_pIds[3] = m_ArtIds;
    m_pDonors[3] = m_ArtDonors;
    m_ItemNums[3] = CollectPart(3, item::KIND_PICTURE);
    m_pNames[5] = &m_ArtNames[m_ItemNums[3]];
    m_pIds[5] = &m_ArtIds[m_ItemNums[3]];
    m_pDonors[5] = &m_ArtDonors[m_ItemNums[3]];
    m_ItemNums[3] += CollectPart(5, item::KIND_SCULPTURE);
    // the second text of the rows
    m_pDonorRenders = static_cast<script::RenderGarden*>(
        ssys::ma::lyt::GetAllocator()->Alloc(m_RowNum * sizeof(script::RenderGarden), 4));
    for (s32 i = 0; i < m_RowNum; i++) {
        new (&m_pDonorRenders[i]) script::RenderGarden(m_DonorLength);
        g_FormatBuffer.format("%s_%02d", "T_name", i);
        m_pDonorRenders[i].SetTextBoxes(m_Layout.FindTextBox(g_FormatBuffer.getCString()), NULL);
    }
    m_ItemNum = m_ItemNums[0];
    return true;
}

// 0x001C6208 slot 0x38
void CatalogList::InitializeScroll()
{
    m_ScrollBar.Scroll(m_Tab.m_Scroll[m_Tab.m_Current]);
}

// 0x001C6040 slot 0x3C
void CatalogList::OnScroll()
{
    const s32 scroll = m_ScrollBar.m_Scroll;
    for (s32 i = 0; i < m_RowNum; i++) {
        const s32 item = i + scroll;
        if (item < m_ItemNums[m_ShownTab]) {
            m_pRenders[i].SetText(&m_pNames[m_ShownTab][item]);
            m_pDonorRenders[i].SetText(&m_pDonors[m_ShownTab][item]);
            SetFlagVisible(m_pContents[i].m_pPane, true);
            SetFlagVisible(m_BaseContents[i].m_pPane, true);
        } else {
            // (the bases of the first six rows stay)
            SetFlagVisible(m_pContents[i].m_pPane, false);
            SetFlagVisible(m_BaseContents[i].m_pPane, i < 6);
        }
        m_pRenders[i].CommitText();
        m_pDonorRenders[i].CommitText();
        // the text box of the donor is as wide as its text
        const f32 height = m_pDonorRenders[i].m_pTextBox->m_Size.height;
        const f32 width = m_pDonorRenders[i].MeasureWidth();
        m_pDonorRenders[i].m_pTextBox->m_Size = nw::lyt::Size(width, height);
    }
}

// 0x001C61D8 slot 0x44
void CatalogList::UpdateItemNum()
{
    const s32 tab = m_Tab.m_Current;
    m_ShownTab = tab;
    m_ItemNum = m_ItemNums[tab];
}

// 0x001C64E4 slot 0x4C
void CatalogList::OnEnterIn()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_In);
    Reset();
}

// 0x001C650C slot 0x54
void CatalogList::OnEnterOut()
{
    m_Tab.Change(&Tab<TAB_NUM>::State_Out);
}

// 0x001C6A84 slot 0x60
void CatalogList::UpdateWait()
{
    // another tab was chosen: the list keeps the scroll of this one and fades to the other
    if (m_Tab.m_Current != m_ShownTab) {
        m_Tab.m_Scroll[m_ShownTab] = m_ScrollBar.m_Scroll;
        Change(&CatalogBase::State_ListFadeOut);
    }
}
