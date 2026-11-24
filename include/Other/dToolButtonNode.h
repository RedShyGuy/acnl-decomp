#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionNode.h"
#include "Other/dLayout.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/lyt_TexMap.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_Color8.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class Material;
class Pane;
class Picture;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// The tool button of the design editor (BsMenuEdit; class name from the RTTI, the state names from the table of the
// binary, all other names are ours): the button B_btn of edit_tool.bclyt opens a window with 12 tools (B_tool_NN);
// the chosen one shows its icon (N_icon_NN) in the picture P_icon of the button.
// RTTI 14ToolButtonNode @ 0x008CBE2C
// vtable 0x008F0980 (vptr 0x008F0988), offset_to_top 0, 16 entries
// vtable 0x008F09C8 (vptr 0x008F09D0), offset_to_top -224, 3 entries
class ToolButtonNode : public ::ButtonActionNode, public ::state::Mode<ToolButtonNode>
{
public:
    static const s32 TOOL_NUM = 12;
    // the index of the node of the button itself in the control (GetPressedIndex)
    static const s32 BUTTON_INDEX_SELF = -2;

    // the arguments of Setup
    struct SetupArg
    {
        ssys::ma::lyt::ArcResAccReader* reader;  // 0x0
        nw::lyt::Pane* pBasePane;                // 0x4 a pane of the layout of BsMenuEdit (kept, not used here)
    };

    ToolButtonNode(); // 0x0027C61C
    // 0x00775EA4 (thunk)
    // 0x00775E40 (deleting thunk)
    // 0x0027C6FC (deleting dtor)
    ~ToolButtonNode() override; // 0x0027C760 slot 0x00

    // the layout, the animations, the tool buttons; the machine starts idle
    void Setup(const SetupArg& arg); // 0x0027BC00
    // the control, the state and the layout
    void Update(); // 0x0027C578

    // the states (the names in the comments are the debug names of the binary)
    void State_Idle();    // 0x0027C4A0 アイドル (idle)
    void Enter_Idle();    // 0x0027C4A4
    void State_In();      // 0x0027C0AC 入場 (the window comes in)
    void Enter_In();      // 0x0027C1B4
    void State_Out();     // 0x0027C250 退場 (the window goes out)
    void Enter_Out();     // 0x0027C2F8
    void State_Select();  // 0x0027C4A8 選択 (choosing a tool)
    void Enter_Select();  // 0x0027C574

private:
    // the icon of the selected tool in P_icon
    void UpdateIcon(); // 0x0027C398

    typedef ::state::Mode<ToolButtonNode>::Info StateInfo;
    static const StateInfo s_StateInfos[4];

public:
    ButtonActionNode m_Tools[TOOL_NUM];       // 0x104 B_tool_NN
    ButtonActionControl m_Control;            // 0xB84 the tools and (while the window is open) the button itself
    ::Layout m_Layout;                        // 0xC68 edit_tool.bclyt
    ssys::ma::lyt::Animation m_InAnim;        // 0xDB4 edit_tool_in
    ssys::ma::lyt::Animation m_OutAnim;       // 0xDDC edit_tool_out
    nw::lyt::Picture* m_pIcon;                // 0xE04 P_icon
    nw::lyt::Material* m_pIconMaterial;       // 0xE08
    // the original icon of P_icon (shown while the window is open)
    nw::lyt::Size m_IconSize;                 // 0xE0C
    nn::math::VEC3 m_IconRotate;              // 0xE14
    nw::lyt::TexMap m_IconTexMap;             // 0xE20
    u32 m_IconTexSRT[5];                      // 0xE40 a copy of the nw::lyt::TexSRT (without its constructor)
    nw::ut::Color8 m_IconColor;               // 0xE54
    nw::lyt::Pane* m_pBasePane;               // 0xE58
    bool m_IsAttached;                        // 0xE5C the layout is a child of the base pane (BsMenuEdit)
    bool m_IsAnimating;                       // 0xE5D the window comes in or goes out
    bool m_IsCursorOnTool;                    // 0xE5E (the selected tool hides its selection)
    s8 m_SelectedTool;                        // 0xE5F
};
ASSERT_OFFSET(ToolButtonNode, m_Tools, 0x104);
ASSERT_OFFSET(ToolButtonNode, m_Control, 0xB84);
ASSERT_OFFSET(ToolButtonNode, m_Layout, 0xC68);
ASSERT_OFFSET(ToolButtonNode, m_pIcon, 0xE04);
ASSERT_OFFSET(ToolButtonNode, m_IconTexMap, 0xE20);
ASSERT_OFFSET(ToolButtonNode, m_IconColor, 0xE54);
ASSERT_OFFSET(ToolButtonNode, m_SelectedTool, 0xE5F);
ASSERT_SIZE(ToolButtonNode, 0xE60);
