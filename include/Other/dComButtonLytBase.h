#pragma once

#include "decomp.h"
#include "Other/dLayout.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

// The base of the layouts of the common buttons (class name from the RTTI, all other names are ours): the layout
// btn_pos.bclyt with the positions N_cmnbtn_pos_L/C/R of the buttons and its reader.
// RTTI 16ComButtonLytBase @ 0x008CC2F0
// vtable 0x008F299C (vptr 0x008F29A4), offset_to_top 0, 2 entries
class ComButtonLytBase
{
public:
    // (inline in the owners)
    ComButtonLytBase() {}
    // 0x002B82C8 (deleting dtor)
    virtual ~ComButtonLytBase(); // 0x002B82F0 slot 0x00

    // the layout is animated and drawn on the bottom screen
    void Draw(); // 0x002B827C
    // reads Layout/common/btn_pos.arc
    bool ReadArchive(); // 0x002B82A0

    // the names of the position panes (left, center, right)
    // 0x00952668
    static const char* s_PositionNames[3];

    ::Layout m_Layout;                           // 0x004 btn_pos.bclyt
    ssys::ma::lyt::ArcResAccReader m_Reader;     // 0x150
};
ASSERT_SIZE(ComButtonLytBase, 0x398);
