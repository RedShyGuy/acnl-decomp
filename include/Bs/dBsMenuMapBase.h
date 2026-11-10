#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dAnimation.h"

class Layout;
namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
class Layout;
} // namespace lyt
} // namespace ma
} // namespace ssys

// The map of the town in the menus (game code, not decompiled yet: only what PWorkDeleteList uses is declared; the
// methods are named after their addresses, the fields are ours). It has its own layout, an in and an out animation
// and the panes of the acres.
// RTTI 13BsMenuMapBase @ 0x008CB934
// vtable 0x008EEFC8 (vptr 0x008EEFD0), offset_to_top 0, 2 entries
class BsMenuMapBase
{
public:
    // the layout of the map (names are ours)
    struct Description
    {
        ssys::ma::lyt::ArcResAccReader* arcReader;  // 0x0
        ::Layout* layout;                           // 0x4
        const char* name;                           // 0x8
    };

    BsMenuMapBase(); // 0x00223694
    virtual ~BsMenuMapBase(); // 0x00223848 slot 0x00
    // 0x00223838 slot 0x04 (deleting dtor)

    void Func2203C0(bool arg); // 0x002203C0
    void Func220410(); // 0x00220410
    void Func2206CC(const char* paneName); // 0x002206CC
    // (loading; true when done)
    bool Func220C7C(); // 0x00220C7C
    void Func221250(u32 arg1, bool arg2); // 0x00221250
    // (a mark at the position of the town)
    void Func221C88(u8 x, u8 y, u32 arg3, bool arg4); // 0x00221C88
    void Func222B3C(); // 0x00222B3C (draws)
    void Func222C88(const Description* description, bool arg); // 0x00222C88
    void Func222E90(); // 0x00222E90 (updates)
    // (the acre of a pane)
    void Func22323C(const char* paneName, u16 acre, u32 arg3); // 0x0022323C
    void Func223478(); // 0x00223478 (finalizes)
    void Func223580(f32 arg1, bool arg2); // 0x00223580

    u8 m_Unknown0x4[0xB2F0];              // 0x0004
    ssys::ma::lyt::Layout* m_pLayout;     // 0xB2F4
    u32 m_Unknown0xB2F8;                  // 0xB2F8
    ssys::ma::lyt::Animation m_InAnim;    // 0xB2FC
    ssys::ma::lyt::Animation m_OutAnim;   // 0xB324
    nw::lyt::Pane* m_pPane;               // 0xB34C
    u8 m_Unknown0xB350[0x56];             // 0xB350
    bool m_Unknown0xB3A6;                 // 0xB3A6
    u8 m_Unknown0xB3A7[0x45];             // 0xB3A7
};
ASSERT_OFFSET(BsMenuMapBase, m_pLayout, 0xB2F4);
ASSERT_OFFSET(BsMenuMapBase, m_InAnim, 0xB2FC);
ASSERT_OFFSET(BsMenuMapBase, m_pPane, 0xB34C);
ASSERT_OFFSET(BsMenuMapBase, m_Unknown0xB3A6, 0xB3A6);
ASSERT_SIZE(BsMenuMapBase, 0xB3EC);
