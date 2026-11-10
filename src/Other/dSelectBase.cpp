#include "Other/dSelectBase.h"

#include "Item/item_Api.h"
#include "Other/dFormatBuffer.h"
#include "Other/dHeapInit.h"
#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "script/dRenderGarden.h"
#include "script/dScriptExtern.h"
#include "sead/seadExpHeap.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

// the item list panes are "<this>_NN"
// 0x009543AC
const char* SelectBase::s_ListPaneName = "N_list";
// the sort tables of the regions
// 0x009543B8
const char* SelectBase::s_SortFiles[10] = {
    "Item/Param/SortJpJpn.bin", "Item/Param/SortUsEng.bin", "Item/Param/SortUsSpa.bin", "Item/Param/SortUsFra.bin",
    "Item/Param/SortEuEng.bin", "Item/Param/SortEuSpa.bin", "Item/Param/SortEuFra.bin", "Item/Param/SortEuIta.bin",
    "Item/Param/SortEuGer.bin", "Item/Param/SortKrKor.bin",
};

namespace {
// the sort table: a sort key per item ID from 0x2000
// 0x00A19690
ssys::ma::LoadSplit s_SortLoader;

// the first item ID of the sort table
const u16 SORT_FIRST_ID = 0x2000;
// the value the item lists pass to Func6F0438/Func6F0608
const u32 ITEM_LIST_VALUE = 178;
} // namespace

// 0x001AF8FC slot 0x40
void SelectBase::OnSelect(s32 selected, s32 prev)
{
    m_Control.UnselectCurrent();
    if (m_pCursorPane->m_pParent != NULL) {
        m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
    }
    const s32 prevRow = prev - m_ScrollBar.m_Scroll;
    if (prevRow >= 0 && prevRow < m_RowNum) {
        m_pNodes[prevRow].Select(false, false);
        SetRowActive(prevRow, true);
    }
    m_Selected = selected;
    const s32 row = selected - m_ScrollBar.m_Scroll;
    if (row >= 0 && row < m_RowNum) {
        ButtonActionNode& node = m_pNodes[row];
        node.m_IsPressed = true;
        node.Animate(node.m_TouchOk, node.m_IsPressed);
        node.Animate(node.m_SelectOk, node.m_IsPressed);
        SetRowActive(row, false);
        m_pContents[row].m_pPane->PrependChild(m_pCursorPane);
    }
}

// 0x001AFA14
void SelectBase::UnloadSortTable()
{
    s_SortLoader.Reset();
}

// 0x001AFA20
void SelectBase::SetRowActive(s32 row, bool active)
{
    if (row < m_RowNum - 2) {
        m_pNodes[row].m_IsActive = active;
    }
}

// 0x001AFA44 slot 0x30
void SelectBase::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "fade_out");
    m_ListOutAnim.Initialize(g_FormatBuffer.getCString(), arcReader);
    g_FormatBuffer.format("%s_%s.bclan", m_LayoutName, "fade_in");
    m_ListInAnim.Initialize(g_FormatBuffer.getCString(), arcReader);

    // the buttons of the rows (copied from the image 0x0087B0D4)
    const ButtonActionControl::Description desc = {
        arcReader,
        &m_Layout,
        NULL,
        "ctlg_cntnt_00_touch.bclan",
        "ctlg_cntnt_00_select.bclan",
        "ctlg_cntnt_00_touch_ok.bclan",
        "ctlg_cntnt_00_select_ok.bclan",
        7,
        NULL,
    };
    m_Control.Initialize(desc, NULL);
    m_Control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_CURSOR;
    m_Control.SetCursor(false);
    m_Control.m_Flags &= ~ButtonActionControl::DISABLED;
    ButtonActionNode::Info info;
    info.pane = NULL;
    info.group = NULL;
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.input = 0;
    info.index = 0;
    info.cursorCallback = NULL;
    for (s32 i = 0; i < m_RowNum; i++) {
        g_FormatBuffer.format("%s_%02d", s_ListPaneName, i);
        nw::lyt::Pane* pane = m_Layout.FindPane(g_FormatBuffer.getCString());
        g_FormatBuffer.format("%s_%02d", "B_list", i);
        info.pane = m_Layout.FindPane(g_FormatBuffer.getCString());
        g_FormatBuffer.format("%s_%02d", "G_list", i);
        info.group = m_Layout.FindGroup(g_FormatBuffer.getCString());
        info.index = i;
        m_pNodes[i].Initialize(info);
        m_pNodes[i].m_SeOk = SE_SYS_SCROLL_LIST_SELECTED;
        m_pNodes[i].m_OkSoundEnabled = true;
        m_pNodes[i].m_Se = SE_SYS_SCROLL_LIST_ACTIVE;
        m_pNodes[i].m_SoundEnabled = true;
        m_pNodes[i].m_SoundMode = false;
        m_pContents[i].m_pPane = pane;
        m_pContents[i].m_BasePosition = pane->m_Translate;
        m_pContents[i].m_BasePosition = m_pContents[i].m_pPane->m_Translate;
        m_ScrollBar.AddContent(&m_pContents[i]);
        m_Control.AddNode(&m_pNodes[i], true);
    }
    m_pCursorPane = m_Layout.FindPane("P_slct_00");
    m_LineNode.m_pPane = m_Layout.FindPane("N_line_00");
    m_LineNode.m_BasePosition = m_LineNode.m_pPane->m_Translate;
    m_LineNode.m_BasePosition = m_LineNode.m_pPane->m_Translate;
    m_ScrollBar.AddContent(&m_LineNode);

    // the cursor gets the end of its select_ok animation, then leaves the layout until an item is selected
    ssys::ma::lyt::Animation anim;
    g_FormatBuffer.format("%s_%s.bclan", "ctlg_cntnt_00", "select_ok");
    anim.Initialize(g_FormatBuffer.getCString(), arcReader);
    m_Layout.BindAnimation(&anim, m_Layout.FindGroup("G_slct_00"), false);
    anim.SetLastFrame();
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&anim, m_Layout.FindGroup("G_slct_00"), false);
    m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
    m_pNodes[m_RowNum - 2].m_IsActive = false;
    m_pNodes[m_RowNum - 1].m_IsActive = false;
}

// 0x001AFED0
void SelectBase::SortIds(u16* ids, s32 num)
{
    const u16* keys = reinterpret_cast<const u16*>(s_SortLoader.m_pData);
    for (s32 i = 0; i < num - 1; i++) {
        u16 key = keys[ids[i] - SORT_FIRST_ID];
        for (s32 j = i + 1; j < num; j++) {
            const u16 id = ids[j];
            if (keys[id - SORT_FIRST_ID] < key) {
                const u16 tmp = ids[i];
                ids[i] = id;
                ids[j] = tmp;
                key = keys[ids[i] - SORT_FIRST_ID];
            }
        }
    }
}

// 0x001AFF6C slot 0x3C
void SelectBase::OnScroll()
{
    const s32 scroll = m_ScrollBar.m_Scroll;
    if (m_pCursorPane->m_pParent != NULL) {
        m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
    }
    for (s32 i = 0; i < m_RowNum; i++) {
        const s32 item = i + scroll;
        if (item < m_ItemNum) {
            m_pRenders[i].SetText(GetItemWord(item));
            m_pRenders[i].CommitText();
            m_pContents[i].m_pPane->SetVisible(true);
            if (item == m_Selected) {
                SetRowActive(i, false);
                ButtonActionNode& node = m_pNodes[i];
                node.m_IsPressed = true;
                node.Animate(node.m_TouchOk, node.m_IsPressed);
                node.Animate(node.m_SelectOk, node.m_IsPressed);
                m_pContents[i].m_pPane->PrependChild(m_pCursorPane);
            } else {
                SetRowActive(i, true);
                m_pNodes[i].Select(false, false);
            }
        } else {
            m_pContents[i].m_pPane->SetVisible(false);
            SetRowActive(i, false);
        }
    }
}

// 0x001B0114
void SelectBase::SortNames(SvFgName* names, s32 num)
{
    const u16* keys = reinterpret_cast<const u16*>(s_SortLoader.m_pData);
    for (s32 i = 0; i < num - 1; i++) {
        u16 key = keys[GetFgNameId(&names[i]) - SORT_FIRST_ID];
        for (s32 j = i + 1; j < num; j++) {
            if (keys[GetFgNameId(&names[j]) - SORT_FIRST_ID] < key) {
                const SvFgName tmp(names[i]);
                names[i] = names[j];
                names[j] = tmp;
                key = keys[GetFgNameId(&names[i]) - SORT_FIRST_ID];
            }
        }
    }
}

// 0x001B01E8 slot 0x48
void SelectBase::ApplyScroll()
{
    CatalogBase::ApplyScroll();
    m_ScrollBar.m_Selected = m_Selected;
}

// 0x001B022C
void SelectBase::AddName(SvFgName* names, s32* num, const SvFgName& name)
{
    SvFgName tmp(name);
    if (Func76B6A0(&tmp)) {
        Func6BA2E8(&tmp);
    } else if (Func76BE54(&tmp)) {
        Func6BBCF0(&tmp);
    }
    for (s32 i = 0; i < *num; i++) {
        const item::Kind kind = tmp.GetKind();
        if (kind == item::KIND_FISH || kind == item::KIND_INSECT) {
            if (Func2FE704(&names[i], &tmp)) {
                return;
            }
        } else if (Func76BA20(&names[i], &tmp)) {
            return;
        }
    }
    names[*num] = tmp;
    (*num)++;
}

// 0x001B032C slot 0x0C
void SelectBase::Finalize()
{
    // the cursor goes back into the layout so that it is freed with it
    if (m_pCursorPane != NULL && m_pCursorPane->m_pParent == NULL) {
        m_pContents[0].m_pPane->AppendChild(m_pCursorPane);
    }
    CatalogBase::Finalize();
}

// 0x001B03B8 slot 0x10
void SelectBase::Update()
{
    m_Control.SetFlags(ButtonActionControl::DISABLED, m_IsScrollBarStopped);
    m_Control.Update();
    m_ScrollBar.m_IsPadEnabled = !(m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing());
    m_ScrollBar.SetFrozen(m_IsScrollBarStopped || m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing());
    if (!m_IsScrollBarStopped) {
        m_ScrollBar.Process();
        if (m_ScrollBar.m_ThumbMove != 0.0f) {
            m_Layout.m_IsAnimationChanged = true;
        }
    }
    Call();
    if (m_ScrollBar.m_IsPadModeEntered == true) {
        m_Control.UnselectCurrent();
    }
}

// 0x001B054C
void SelectBase::LoadSortTable()
{
    // 0x009543B4 (guard 0x009543B0)
    static const char* s_pSortFile = s_SortFiles[GetRegion()];
    s_SortLoader.Read(sead::SafeStringBase<char>(s_pSortFile), HeapInit::s_pStageHeap, 128);
}

// 0x001B05CC
void SelectBase::SetNames(script::WordFix<33>* words, const u16* ids, u32 num)
{
    for (u32 i = 0; i < num; i++) {
        SvFgName name(static_cast<SvFgName::ID>(ids[i]));
        item::GetName(&words[i], name, NULL);
    }
}

// 0x001B062C
s32 SelectBase::CollectIds(u16* ids, u8 group, bool isSome)
{
    const u32* bits = g_pSvCurrentPlayer->unlockedItems;
    u8 list[7104];
    Func6F05B8(list);
    if (isSome) {
        Func6F0438(list, bits, group, ITEM_LIST_VALUE);
    } else {
        Func6F02F4(list, bits, group);
        if (group == 5) {
            Func6F0608(list, ITEM_LIST_VALUE);
        }
    }
    const s32 num = Func7712C0(list);
    s32 count = 0;
    for (s32 i = 0; i < num; i++) {
        SvFgName name(Func771278(list, group, i));
        if (Func76A66C(&name)) {
            *ids++ = GetFgNameId(&name);
            count++;
        }
    }
    return count;
}

// 0x001B073C slot 0x60
void SelectBase::UpdateWait()
{
    if (m_Control.IsSelectOkDone()) {
        // a row was chosen: it becomes the selection
        if (m_pCursorPane->m_pParent != NULL) {
            m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
        }
        if (m_Selected != -1) {
            const s32 row = m_Selected - m_ScrollBar.m_Scroll;
            if (row >= 0 && row < m_RowNum) {
                m_pNodes[row].Select(false, false);
                SetRowActive(row, true);
            }
        }
        m_Selected = m_Control.GetPressedIndex() + m_ScrollBar.m_Scroll;
        SetRowActive(m_Control.GetPressedIndex(), false);
        m_pContents[m_Control.GetPressedIndex()].m_pPane->PrependChild(m_pCursorPane);
        m_ScrollBar.m_Selected = m_Selected;
        m_Control.Reset();
    }
    if (m_Selected != -1) {
        const s32 row = m_Selected - m_ScrollBar.m_Scroll;
        if (row >= 0 && row < m_RowNum) {
            SetRowActive(row, false);
        }
    }
}

// 0x001B087C
SelectBase::SelectBase() : m_pNodes(NULL), m_pCursorPane(NULL), m_Selected(-1)
{
    m_LayoutName = "ctlg_cntnt_00";
}

// 0x001B099C slot 0x00
// 0x001B08E0 (deleting dtor)
SelectBase::~SelectBase()
{
    // (only the members are destroyed)
}
