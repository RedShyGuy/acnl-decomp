#pragma once

#include "decomp.h"
#include "Other/dComButtonLytBase.h"
#include "Other/dControl2Button.h"

// The layout of 2 common buttons (class name from the RTTI, all other names are ours).
// RTTI 13ComButton2Lyt @ 0x008CB9C4
// vtable 0x008EF250 (vptr 0x008EF258), offset_to_top 0, 2 entries
class ComButton2Lyt : public ::ComButtonLytBase
{
public:
    // (inline in the owners)
    ComButton2Lyt() {}
    // 0x0022C518 (deleting dtor)
    ~ComButton2Lyt() override; // 0x0022C58C slot 0x00

    // the layout and the buttons on their positions
    bool Setup(); // 0x0022C45C
    // the buttons leave their panes
    bool Finalize(); // 0x0022C4F0
    // the buttons and their state
    void Update(); // 0x0022C510
    // both buttons on their positions
    void ShowBoth(); // 0x0022C3AC
    // only the second button, on the position
    void ShowOne(s32 position); // 0x0022C418

    Control2Button m_Control; // 0x398
};
ASSERT_SIZE(ComButton2Lyt, 0x398 + sizeof(Control2Button));
