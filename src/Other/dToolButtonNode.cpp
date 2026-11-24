#include "Other/dToolButtonNode.h"

#include <cstring>
#include "Other/dFormatBuffer.h"
#include "Sound/dSoundLabel.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x10000;
// the layer of the layout (in front of the editor)
const u8 LAYOUT_LAYER = 156;
// the frames of the cursor before repeating
const s32 CURSOR_TIMER = 9;
} // namespace

// the states with their names (Japanese, debug); the machine starts idle
// 0x0084A77C
const ToolButtonNode::StateInfo ToolButtonNode::s_StateInfos[4] = {
    {"アイドル", &ToolButtonNode::Enter_Idle, &ToolButtonNode::State_Idle},
    {"入場", &ToolButtonNode::Enter_In, &ToolButtonNode::State_In},
    {"退場", &ToolButtonNode::Enter_Out, &ToolButtonNode::State_Out},
    {"選択", &ToolButtonNode::Enter_Select, &ToolButtonNode::State_Select},
};

// 0x0027C61C
ToolButtonNode::ToolButtonNode()
    : m_pIcon(NULL), m_pIconMaterial(NULL), m_pBasePane(NULL), m_IsAttached(false), m_IsAnimating(false),
      m_IsCursorOnTool(false), m_SelectedTool(0)
{
    m_pLayout = &m_Layout;
}

// 0x0027C760 slot 0x00
// 0x00775EA4 (thunk)
// 0x00775E40 (deleting thunk)
// 0x0027C6FC (deleting dtor)
ToolButtonNode::~ToolButtonNode()
{
    // (only the members are destroyed)
}

// 0x0027BC00
void ToolButtonNode::Setup(const SetupArg& arg)
{
    m_Layout.m_pArcReader = arg.reader;
    m_Layout.Build("edit_tool.bclyt", NULL, CMD_CACHE_SIZE);
    ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, BUTTON_INDEX_SELF, NULL};
    info.pane = m_Layout.FindBounding("B_btn");
    info.group = m_Layout.FindGroup("G_btn");
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    Initialize(info);
    m_pBasePane = arg.pBasePane;
    m_InAnim.Initialize("edit_tool_in.bclan", arg.reader);
    m_OutAnim.Initialize("edit_tool_out.bclan", arg.reader);
    m_Touch.Initialize("edit_tool_touch.bclan", arg.reader);
    m_Select.Initialize("edit_tool_select.bclan", arg.reader);
    m_TouchOk.Initialize("edit_tool_touch_ok.bclan", arg.reader);
    m_SelectOk.Initialize("edit_tool_select_ok.bclan", arg.reader);
    // the window starts closed
    m_Layout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetLastFrame();
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&m_OutAnim);
    m_Layout.m_Layer = LAYOUT_LAYER;

    m_pIcon = m_Layout.FindPicture("P_icon");
    m_pIconMaterial = m_pIcon->GetMaterial(0);
    m_IconSize = m_pIcon->m_Size;
    m_IconRotate = m_pIcon->m_Rotate;
    m_IconTexMap = m_pIconMaterial->GetTexMapAry()[0];
    std::memcpy(m_IconTexSRT, static_cast<const nw::lyt::Material*>(m_pIconMaterial)->GetTexSRTAry(),
                sizeof(m_IconTexSRT));
    m_IconColor = m_pIconMaterial->m_Colors[0];
    UpdateIcon();

    ButtonActionControl::Description desc = {arg.reader,
                                             m_pLayout,
                                             NULL,
                                             "edit_tool_touch.bclan",
                                             "edit_tool_select.bclan",
                                             "edit_tool_touch_ok.bclan",
                                             "edit_tool_select_ok.bclan",
                                             CURSOR_TIMER,
                                             NULL};
    m_Control.Initialize(desc, NULL);
    m_Control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;

    ButtonActionNode::Info toolInfo;
    toolInfo.pane = NULL;
    toolInfo.group = NULL;
    toolInfo.pos = nn::math::VEC2(0.0f, 0.0f);
    toolInfo.input = 0;
    toolInfo.index = 0;
    toolInfo.cursorCallback = NULL;
    for (s32 i = 0; i < TOOL_NUM; i++) {
        g_FormatBuffer.format("%s_%02d", "B_tool", i);
        toolInfo.pane = m_Layout.FindBounding(g_FormatBuffer.getCString());
        g_FormatBuffer.format("%s_%02d", "G_tool", i);
        toolInfo.group = m_Layout.FindGroup(g_FormatBuffer.getCString());
        toolInfo.index = i;
        m_Tools[i].Initialize(toolInfo);
        m_Tools[i].SetSeOk(SE_SYS_DECIDE_S);
        m_Tools[i].SetOkSoundEnabled(true);
        m_Tools[i].SetSe(SE_SYS_EDIT_BTN_ACTIVE);
        m_Tools[i].SetSoundEnabled(true);
        m_Tools[i].m_SoundMode = true;
        m_Control.AddNode(&m_Tools[i], true);
    }
    Start("BsMenuEdit::ToolButtonNode", s_StateInfos, 4);
    SetSeOk(SE_SYS_EDIT_TOOL_WIN_OPEN);
    SetOkSoundEnabled(true);
    SetSe(SE_SYS_EDIT_BTN_ACTIVE);
    SetSoundEnabled(true);
    m_SoundMode = true;
}

// 0x0027C0AC
void ToolButtonNode::State_In()
{
    if (!m_InAnim.IsDone()) {
        m_InAnim.IncreaseFrame();
        return;
    }
    m_Layout.UnbindAnimation(&m_InAnim);
    m_IsAnimating = false;
    // the button shows its own icon while the window is open
    m_pIcon->m_Size = m_IconSize;
    m_pIcon->m_Rotate = m_IconRotate;
    m_pIcon->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
    m_pIconMaterial->SetTexMap(0, &m_IconTexMap);
    std::memcpy(m_pIconMaterial->GetTexSRTAry(), m_IconTexSRT, sizeof(m_IconTexSRT));
    m_pIconMaterial->SetCacheDirty();
    m_pIconMaterial->m_Colors[0] = m_IconColor;
    ButtonActionNode::Select(false);
    // the button closes the window again
    m_Control.AddNode(this, false);
    m_Control.SetFlags(ButtonActionControl::DISABLED, false);
    Change(&ToolButtonNode::State_Select);
}

// 0x0027C1B4
void ToolButtonNode::Enter_In()
{
    m_Control.Reset();
    m_Control.ResetSelectAll();
    m_Control.UnselectActive();
    m_Layout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_IsAnimating = true;
    m_Tools[m_SelectedTool].ButtonActionNode::Select(true);
    m_Tools[m_SelectedTool].Deactivate();
    SetSeOk(SE_SYS_EDIT_TOOL_WIN_CLOSE);
    SetOkSoundEnabled(true);
}

// 0x0027C250
void ToolButtonNode::State_Out()
{
    if (!m_OutAnim.IsDone()) {
        m_OutAnim.IncreaseFrame();
        return;
    }
    m_Layout.UnbindAnimation(&m_OutAnim);
    m_IsAnimating = false;
    if (m_Control.GetPressedIndex() == BUTTON_INDEX_SELF) {
        ButtonActionNode::Select(false);
    } else {
        m_Tools[m_SelectedTool].ButtonActionNode::Select(false);
    }
    m_Control.Erase(this);
    Change(&ToolButtonNode::State_Idle);
}

// 0x0027C2F8
void ToolButtonNode::Enter_Out()
{
    m_Tools[m_SelectedTool].Activate();
    m_SelectedTool = m_Control.GetPressedIndex() == BUTTON_INDEX_SELF ? m_SelectedTool
                                                                      : static_cast<s8>(m_Control.GetPressedIndex());
    UpdateIcon();
    m_Layout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_Control.SetFlags(ButtonActionControl::DISABLED, true);
    m_IsAnimating = true;
    SetSeOk(SE_SYS_EDIT_TOOL_WIN_OPEN);
    SetOkSoundEnabled(true);
}

// 0x0027C398
void ToolButtonNode::UpdateIcon()
{
    g_FormatBuffer.format("%s_%02d", "N_icon", m_SelectedTool);
    nw::lyt::Picture* icon = m_Layout.FindPicture(g_FormatBuffer.getCString());
    nw::lyt::Material* material = icon->GetMaterial(0);
    m_pIcon->m_Rotate = icon->m_Rotate;
    m_pIcon->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
    m_pIcon->m_Size = icon->m_Size;
    m_pIconMaterial->SetTexMap(0, &material->GetTexMapAry()[0]);
    *m_pIconMaterial->GetTexSRTAry() = *static_cast<const nw::lyt::Material*>(material)->GetTexSRTAry();
    m_pIconMaterial->SetCacheDirty();
    m_pIconMaterial->m_Colors[0] = material->m_Colors[0];
    m_Layout.Animate();
}

// 0x0027C4A0
void ToolButtonNode::State_Idle()
{
    // empty (in the original too)
}

// 0x0027C4A4
void ToolButtonNode::Enter_Idle()
{
    // empty (in the original too)
}

// 0x0027C4A8
void ToolButtonNode::State_Select()
{
    if (m_Control.IsSelectOkDone()) {
        return;
    }
    if (m_Control.HasCurrent()) {
        if (!m_IsCursorOnTool) {
            if (m_Control.GetSelectedIndex() < TOOL_NUM) {
                m_Tools[m_SelectedTool].Deactivate();
                m_Tools[m_SelectedTool].ButtonActionNode::Select(false);
            }
            m_IsCursorOnTool = true;
        }
    } else if (m_IsCursorOnTool) {
        m_Tools[m_SelectedTool].Select(true, false);
        m_Tools[m_SelectedTool].ButtonActionNode::Select(true);
        m_IsCursorOnTool = false;
    }
}

// 0x0027C574
void ToolButtonNode::Enter_Select()
{
    // empty (in the original too)
}

// 0x0027C578
void ToolButtonNode::Update()
{
    m_Control.Update();
    Call();
    m_Layout.Animate();
}
