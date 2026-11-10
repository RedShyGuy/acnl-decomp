#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionCstmNode.h"
#include "Other/dFormatBuffer.h"
#include "Other/dLayout.h"
#include "Sound/dSoundLabel.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "state/dMode.h"

// The tabs above the lists of the catalog-like menus: N buttons (layout "ctlg_tab" each, hung into the panes N_pos_NN
// of the layout "ctlg_tabpos") that are animated in and out together; the list keeps the selection and the scroll of
// each tab. A state machine (do nothing, enter, exit, wait) whose state names are in the binary. Only the class name
// is from the RTTI; all other names are ours.
// Instantiations found in the binary:
//   Tab<2>  typeinfo 0x008CD154  vtable 0x008F8310
//   Tab<4>  typeinfo 0x008CD160  vtable 0x008F8324
//   Tab<7>  typeinfo 0x008CD16C  vtable 0x008F8338
template <int N>
class Tab : public ::state::Mode<Tab<N> >
{
public:
    typedef typename ::state::Mode<Tab<N> >::Func Func;
    typedef typename ::state::Mode<Tab<N> >::Info Info;

    // (inline, in the lists that have tabs)
    Tab() : m_IsAnimating(false), m_Current(0) {}
    virtual ~Tab();
    const char* GetName(Func func) const override { return this->FindName(func); }

    // the layouts, animations and buttons of the tabs (inline in the lists; Tab<4>, which two lists use, has a copy:
    // see the specialization below)
    DECOMP_ALWAYS_INLINE void Initialize(ssys::ma::lyt::ArcResAccReader* arcReader) { InitializeImpl(arcReader); }
    // (inline) the body of Initialize
    DECOMP_ALWAYS_INLINE void InitializeImpl(ssys::ma::lyt::ArcResAccReader* arcReader);
    // (inline) the buttons and the state; the layout of the positions is animated
    void Update(bool isDisabled)
    {
        m_Control.SetFlags(ButtonActionControl::DISABLED, isDisabled);
        m_Control.Update();
        this->Call();
        m_Layout.Animate();
    }
    // (inline) a tab is being chosen
    bool IsBusy() const { return m_Control.IsSelectOkDone() || m_Control.IsSelectOkOngoing(); }

    // the states
    void State_None();
    void Enter_None();
    void State_In();
    void Enter_In();
    void State_Out();
    void Enter_Out();
    void State_Wait();
    void Enter_Wait();

    static const Info s_StateInfos[4];

    ::Layout m_Layout;                          // 0x024 ctlg_tabpos
    ::Layout m_TabLayouts[N];                   // 0x170 ctlg_tab, one per tab
    ssys::ma::lyt::Animation m_InAnims[N];      // ctlg_tab_in
    ssys::ma::lyt::Animation m_OutAnims[N];     // ctlg_tab_out
    ButtonActionCstmNode m_Nodes[N];            // B_itm_00 / G_tab_00 of each tab
    ButtonActionControl m_Control;
    bool m_IsAnimating;                         // in or out
    s32 m_Current;                              // the tab shown
    s32 m_Selected[N];                          // the selection of each tab (-1: none)
    s32 m_Scroll[N];                            // the scroll of each tab
};
ASSERT_OFFSET(Tab<4>, m_TabLayouts, 0x170);
ASSERT_OFFSET(Tab<4>, m_Control, 0xB60);
ASSERT_OFFSET(Tab<4>, m_Current, 0xC48);
ASSERT_SIZE(Tab<4>, 0xC6C);
ASSERT_OFFSET(Tab<7>, m_Control, 0x12D4);
ASSERT_OFFSET(Tab<7>, m_Selected, 0x13C0);

template <int N>
DECOMP_ALWAYS_INLINE void Tab<N>::InitializeImpl(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    m_Layout.m_pArcReader = arcReader;
    g_FormatBuffer.format("%s.bclyt", "ctlg_tabpos");
    m_Layout.Build(g_FormatBuffer.getCString(), NULL, 0x6000);
    m_Layout.SetLayer(3);
    for (s32 i = 0; i < N; i++) {
        g_FormatBuffer.format("%s_%s.bclan", "ctlg_tab", "in");
        m_InAnims[i].Initialize(g_FormatBuffer.getCString(), arcReader);
        g_FormatBuffer.format("%s_%s.bclan", "ctlg_tab", "out");
        m_OutAnims[i].Initialize(g_FormatBuffer.getCString(), arcReader);
    }
    // the buttons of the tabs
    const ButtonActionControl::Description controlDescription = {arcReader, &m_Layout, NULL, NULL, NULL, NULL, NULL, 7, NULL};
    m_Control.Initialize(controlDescription, NULL);
    m_Control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_CURSOR;
    ButtonActionCstmNode::Description nodeDescription = {
        arcReader,
        NULL,
        "ctlg_tab_touch.bclan",
        "ctlg_tab_select.bclan",
        "ctlg_tab_touch_ok.bclan",
        "ctlg_tab_select_ok.bclan",
        {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, -1, NULL},
    };
    for (s32 i = 0; i < N; i++) {
        m_TabLayouts[i].m_pArcReader = arcReader;
        g_FormatBuffer.format("%s.bclyt", "ctlg_tab");
        m_TabLayouts[i].ssys::ma::lyt::Layout::Build(g_FormatBuffer.getCString(), NULL, 4);
        g_FormatBuffer.format("%s_%02d", "N_pos", i);
        m_Layout.FindPane(g_FormatBuffer.getCString())->AppendChild(m_TabLayouts[i].GetRootPane());
        nodeDescription.layout = &m_TabLayouts[i];
        nodeDescription.info.pane = m_TabLayouts[i].FindPane("B_itm_00");
        nodeDescription.info.group = m_TabLayouts[i].FindGroup("G_tab_00");
        nodeDescription.info.index = i;
        m_Nodes[i].Initialize(nodeDescription);
        m_Nodes[i].m_SeOk = SE_SYS_DECIDE_S;
        m_Nodes[i].m_OkSoundEnabled = true;
        m_Control.AddNode(&m_Nodes[i], false);
        m_Selected[i] = -1;
        m_Scroll[i] = 0;
    }
    // the first tab is shown
    m_Current = 0;
    m_Nodes[0].m_IsActive = false;
    m_Nodes[0].m_IsPressed = true;
    m_Nodes[0].Animate(m_Nodes[0].m_TouchOk, true);
    m_Nodes[0].Animate(m_Nodes[0].m_SelectOk, m_Nodes[0].m_IsPressed);
    this->Start("Tab", s_StateInfos, 4);
}

// (the copy of Tab<4>::Initialize: FtrList and CatalogList call it)
template <>
DECOMP_NOINLINE void Tab<4>::Initialize(ssys::ma::lyt::ArcResAccReader* arcReader); // 0x007EB224
