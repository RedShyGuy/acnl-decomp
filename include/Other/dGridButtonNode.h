#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "Other/dLayout.h"
#include "ssys/ma/lyt/dAnimation.h"

namespace nw {
namespace lyt {
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

// The grid switch of the design editor (BsMenuEdit; class name from the RTTI, all other names are ours): the button
// B_switch of edit_swich.bclyt with two sets of animations (A: grid off, B: grid on); the base byte 0xDE is its state.
// RTTI 14GridButtonNode @ 0x008CBCE0
// vtable 0x008F038C (vptr 0x008F0394), offset_to_top 0, 16 entries
class GridButtonNode : public ::ButtonActionNode
{
public:
    // the arguments of Setup
    struct SetupArg
    {
        ssys::ma::lyt::ArcResAccReader* reader;  // 0x0
        nw::lyt::Pane* pBasePane;                // 0x4 the layout of the button becomes its child
    };

    // (inline in BsMenuEdit)
    GridButtonNode() { m_Unknown0xDE = false; m_pLayout = &m_SwitchLayout; }
    // 0x0026BB6C (deleting dtor)
    ~GridButtonNode() override; // 0x0026BBD0 slot 0x00

    // the layout and the animations; the grid starts off
    void Setup(const SetupArg& arg); // 0x0026B95C
    // the grid is switched (the animations and the ok sound of the other state)
    void Toggle(); // 0x0026BABC
    // whether the grid is on
    bool IsGridOn() const { return m_Unknown0xDE; }

    ssys::ma::lyt::Animation m_TouchB;       // 0x0E0 edit_swich_B_* (grid on)
    ssys::ma::lyt::Animation m_SelectB;      // 0x108
    ssys::ma::lyt::Animation m_TouchOkB;     // 0x130
    ssys::ma::lyt::Animation m_SelectOkB;    // 0x158
    ssys::ma::lyt::Animation m_TouchA;       // 0x180 edit_swich_A_* (grid off)
    ssys::ma::lyt::Animation m_SelectA;      // 0x1A8
    ssys::ma::lyt::Animation m_TouchOkA;     // 0x1D0
    ssys::ma::lyt::Animation m_SelectOkA;    // 0x1F8
    ::Layout m_SwitchLayout;                 // 0x220 edit_swich.bclyt
};
ASSERT_SIZE(GridButtonNode, 0x36C);
