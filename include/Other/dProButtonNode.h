#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "Other/dDragIconControl.h"
#include "Other/dDragIconGroup.h"
#include "Other/dLayout.h"
#include "Other/dMyDesignProDragIconNode.h"
#include "script/dChoiceStandard.h"
#include "script/dChoiceStandardItem.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class Group;
class Pane;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// The pro design button of the design editor (BsMenuEdit; class name from the RTTI, the state names from the table of
// the binary, all other names are ours): for the wearable designs (pattern types 0..5) the parts of the design (front,
// back and, with sleeves, the two sleeves) are icons in edit_pro_M.bclyt; one is edited at a time, and a part dragged
// onto another one is copied or swapped after a choice.
// RTTI 13ProButtonNode @ 0x008CBA84
// vtable 0x008EF714 (vptr 0x008EF71C), offset_to_top 0, 17 entries
// vtable 0x008EF760 (vptr 0x008EF768), offset_to_top -224, 3 entries
class ProButtonNode : public ::ButtonActionNode, public ::state::Mode<ProButtonNode>
{
public:
    // the parts of a design (front, back, left sleeve, right sleeve)
    static const s32 PART_NUM = 4;
    static const s32 PATTERN_TYPE_NUM = 6;
    // the index of the node of the button itself in its control
    static const s32 BUTTON_INDEX_SELF = -3;
    // the results of the choice after a drop
    enum ChoiceResult
    {
        CHOICE_SWAP = 0,
        CHOICE_COPY = 1,
        CHOICE_CANCEL = 2,
    };

    // the arguments of Setup
    struct SetupArg
    {
        ssys::ma::lyt::ArcResAccReader* reader;  // 0x0
        nw::lyt::Pane* pBasePane;                // 0x4 the layout of the button becomes its child
    };

    ProButtonNode(); // 0x0023FFEC
    // 0x00775E30 (thunk)
    // 0x00775E1C (deleting thunk)
    // 0x00240108 (deleting dtor)
    ~ProButtonNode() override; // 0x00240118 slot 0x00

    // whether the parts are not idle (the machine works on something else than the parts, or one is dragged)
    virtual bool IsBusy() // 0x007146A0 slot 0x40
    {
        if (GetState() != &ProButtonNode::State_Exec) {
            return true;
        }
        return m_DragControl.m_pFunc != &ButtonActionControl::State_Normal || m_DragControl.m_pCurrent != NULL;
    }

    // the layout, the animations, the parts and the choice; the machine starts idle
    bool Setup(const SetupArg& arg); // 0x0023EF64
    // the drag control and the state
    void Update(); // 0x0023FF54

    // the states (the names in the comments are the debug names of the binary)
    void State_Idle();    // 0x0023F940 アイドル (idle)
    void Enter_Idle();    // 0x0023FB58
    void State_In();      // 0x0023F52C 入場処理 (the parts come in)
    void Enter_In();      // 0x0023F688
    void State_Out();     // 0x0023F7DC 退場処理 (the parts go out)
    void Enter_Out();     // 0x0023F83C
    void State_Exec();    // 0x0023F944 実行処理 (choosing and dragging the parts)
    void Enter_Exec();    // 0x0023FB5C
    void State_Choice();  // 0x0023FB60 選択肢処理 (the choice after a drop)
    void Enter_Choice();  // 0x0023FC40

private:
    // the panes of the layout and the parts of the pattern type (textures, icons, panes)
    void SetupParts(ssys::ma::lyt::ArcResAccReader* reader); // 0x0023E9CC
    // the part to receives the pixels of the part from (outside of it: the transparent color)
    void CopyPart(s32 to, s32 from); // 0x0023FCC0
    // the two parts exchange their pixels
    void SwapParts(s32 a, s32 b); // 0x0023FDDC

    typedef ::state::Mode<ProButtonNode>::Info StateInfo;
    static const StateInfo s_StateInfos[5];
    // 0x009509B0 the icons of the parts on the button
    static const char* s_IconNames[PART_NUM];
    // 0x009509C0 the buttons of the front and the back (one-piece, body)
    static const char* s_BodyButtonNames[2][2];
    // 0x009509D0 the buttons of the sleeves (long, short)
    static const char* s_SleeveButtonNames[2][2];
    // 0x00950950 the pane of the editing cursor of each part per pattern type
    static const char* s_CursorPaneNames[PATTERN_TYPE_NUM][PART_NUM];

public:
    ssys::ma::HeapAllocator m_Allocator;                // 0x0104 "ProButtonNode" (the textures of the parts)
    ::Layout m_ProLayout;                               // 0x010C edit_pro_M.bclyt
    ssys::ma::lyt::Animation m_InAnim;                  // 0x0258 edit_pro_M_in
    ssys::ma::lyt::Animation m_OutAnim;                 // 0x0280 edit_pro_M_out
    ssys::ma::lyt::Animation m_KindAnim;                // 0x02A8 edit_pro_M_kind (the pattern type)
    ssys::ma::lyt::Animation m_CursorKindAnim;          // 0x02D0 edit_pro_M_crsl_kind (the edited / target part)
    ssys::ma::lyt::Animation m_UnusedAnim;              // 0x02F8 (never initialized)
    DragIconControl m_DragControl;                      // 0x0320
    DragIconGroup m_PartGroup;                          // 0x0438
    MyDesignProDragIconNode m_Parts[PART_NUM];          // 0x0578
    nw::lyt::Pane* m_pBasePane;                         // 0x0FC8
    nw::lyt::Pane* m_pBackPane;                         // 0x0FCC P_back_00
    nw::lyt::Pane* m_pIcons[PART_NUM];                  // 0x0FD0 P_icon_F/B/L/R
    nw::lyt::Pane* m_pCursorPane;                       // 0x0FE0 P_editing_crsl
    nw::lyt::Pane* m_pAllPane;                          // 0x0FE4 N_All
    nw::lyt::Pane* m_pSetPane;                          // 0x0FE8 N_set
    nw::lyt::Pane* m_pPosPane;                          // 0x0FEC N_pos
    nw::lyt::Group* m_pCursorGroup;                     // 0x0FF0 G_crsl
    nw::lyt::Group* m_pEditingGroup;                    // 0x0FF4 G_editing
    s32 m_EditedPart;                                   // 0x0FF8
    s32 m_PatternType;                                  // 0x0FFC (SvMyDesign::patternType)
    s32 m_SleeveKind;                                   // 0x1000 pattern type % 3 (0 long, 1 short, 2 none)
    u32 m_PartNum;                                      // 0x1004 2 without sleeves
    s32 m_TargetPart;                                   // 0x1008 the part under the dragged one (-1: none)
    bool m_IsAnimating;                                 // 0x100C
    bool m_IsChosen;                                    // 0x100D a part was chosen (BsMenuEdit takes it)
    bool m_IsAttached;                                  // 0x100E the layout is a child of the base pane
    u8 m_Unknown0x100F;                                 // 0x100F (never accessed)
    bool m_IsShown;                                     // 0x1010 between coming in and going out
    script::ChoiceStandardItem m_Choice;                // 0x1014 copy / swap / cancel
    script::ChoiceLabelData m_ChoiceLabels[3];          // 0x1C78
};
ASSERT_OFFSET(ProButtonNode, m_Allocator, 0x104);
ASSERT_OFFSET(ProButtonNode, m_DragControl, 0x320);
ASSERT_OFFSET(ProButtonNode, m_PartGroup, 0x438);
ASSERT_OFFSET(ProButtonNode, m_Parts, 0x578);
ASSERT_OFFSET(ProButtonNode, m_pBasePane, 0xFC8);
ASSERT_OFFSET(ProButtonNode, m_EditedPart, 0xFF8);
ASSERT_OFFSET(ProButtonNode, m_Choice, 0x1014);
ASSERT_SIZE(ProButtonNode, 0x1CB4);
