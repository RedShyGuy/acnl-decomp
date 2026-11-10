#include "lyt/dTourList.h"

#include "Other/dFormatBuffer.h"
#include "Sound/dSoundLabel.h"
#include "script/dStrMgr.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace lyt {
// (a global of the tour code)
// 0x0095D734
void* g_pUnknown95D734 = NULL;
} // namespace lyt

// 0x0050C51C slot 0x30
void lyt::TourList::InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    m_Layout.m_Layer = 128;
    // the buttons of the rows (copied from the image 0x00854BA8)
    const ButtonActionControl::Description desc = {
        arcReader,
        &m_Layout,
        NULL,
        "tour_list_cntnt_touch.bclan",
        "tour_list_cntnt_select.bclan",
        "tour_list_cntnt_touch_ok.bclan",
        "tour_list_cntnt_select_ok.bclan",
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
        g_FormatBuffer.format("%s_%02d", "N_list", i);
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
        m_pNodes[i].m_SoundMode = true;
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

    // the cursor gets the end of its select_ok animation, then leaves the layout until a tour is selected
    ssys::ma::lyt::Animation anim;
    g_FormatBuffer.format("%s_%s.bclan", "tour_list_cntnt", "select_ok");
    anim.Initialize(g_FormatBuffer.getCString(), arcReader);
    m_Layout.BindAnimation(&anim, m_Layout.FindGroup("G_slct_00"), false);
    anim.SetLastFrame();
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&anim, m_Layout.FindGroup("G_slct_00"), false);
    m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
    m_pNodes[m_RowNum - 2].m_IsActive = false;
    m_pNodes[m_RowNum - 1].m_IsActive = false;
}

// 0x0050C90C slot 0x34
bool lyt::TourList::SetupItems()
{
    if (g_pUnknown95D734 == NULL) {
        m_ItemNum = 0;
        return true;
    }
    m_ItemNum = Func76FD4C(g_pUnknown95D734);
    for (u32 i = 0; i < static_cast<u32>(m_ItemNum); i++) {
        const u32 id = Func76FD78(g_pUnknown95D734, i);
        script::StrMgr::s_pInstance->GetString(&m_Names[i], "STR_Tour_name", id);
    }
    return true;
}

// 0x0050C9E0 slot 0x00
// 0x0050C9A0 (deleting dtor)
lyt::TourList::~TourList()
{
    // (only the members and the base are destroyed)
}
