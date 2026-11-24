#include "Other/dGridButtonNode.h"

#include "Sound/dSoundLabel.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 4;
// the index of the node of the switch in the controls of BsMenuEdit
const s32 BUTTON_INDEX = 3;
} // namespace

// 0x0026B95C
void GridButtonNode::Setup(const SetupArg& arg)
{
    m_SwitchLayout.m_pArcReader = arg.reader;
    m_SwitchLayout.Build("edit_swich.bclyt", NULL, CMD_CACHE_SIZE);
    ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, BUTTON_INDEX, NULL};
    info.pane = m_SwitchLayout.FindBounding("B_switch");
    info.group = m_SwitchLayout.FindGroup("G_switch");
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    Initialize(info);
    arg.pBasePane->AppendChild(m_SwitchLayout.GetRootPane());
    m_TouchB.Initialize("edit_swich_B_touch.bclan", arg.reader);
    m_SelectB.Initialize("edit_swich_B_select.bclan", arg.reader);
    m_TouchOkB.Initialize("edit_swich_B_touch_ok.bclan", arg.reader);
    m_SelectOkB.Initialize("edit_swich_B_select_ok.bclan", arg.reader);
    m_TouchA.Initialize("edit_swich_A_touch.bclan", arg.reader);
    m_SelectA.Initialize("edit_swich_A_select.bclan", arg.reader);
    m_TouchOkA.Initialize("edit_swich_A_touch_ok.bclan", arg.reader);
    m_SelectOkA.Initialize("edit_swich_A_select_ok.bclan", arg.reader);
    m_Unknown0xDE = true;
    Toggle();
    SetSe(SE_SYS_EDIT_BTN_ACTIVE);
    SetSoundEnabled(true);
    m_SoundMode = true;
}

// 0x0026BABC
void GridButtonNode::Toggle()
{
    m_Unknown0xDE = !m_Unknown0xDE;
    if (m_Unknown0xDE) {
        m_Touch.SetAnimTransform(m_TouchB.GetAnimTransform());
        m_Select.SetAnimTransform(m_SelectB.GetAnimTransform());
        m_TouchOk.SetAnimTransform(m_TouchOkB.GetAnimTransform());
        m_SelectOk.SetAnimTransform(m_SelectOkB.GetAnimTransform());
        SetSeOk(SE_SYS_EDIT_GRID_OFF);
        SetOkSoundEnabled(true);
        return;
    }
    m_Touch.SetAnimTransform(m_TouchA.GetAnimTransform());
    m_Select.SetAnimTransform(m_SelectA.GetAnimTransform());
    m_TouchOk.SetAnimTransform(m_TouchOkA.GetAnimTransform());
    m_SelectOk.SetAnimTransform(m_SelectOkA.GetAnimTransform());
    SetSeOk(SE_SYS_EDIT_GRID_ON);
    SetOkSoundEnabled(true);
}

// 0x0026BBD0 slot 0x00
// 0x0026BB6C (deleting dtor)
GridButtonNode::~GridButtonNode()
{
    // (only the members are destroyed)
}
