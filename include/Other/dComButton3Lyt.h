#pragma once

#include "decomp.h"
#include "Other/dComButtonLytBase.h"
#include "Other/dControl3Button.h"

// The layout of 3 common buttons (class name from the RTTI, all other names are ours).
// RTTI 13ComButton3Lyt @ 0x008CB9D0
// vtable 0x008EF260 (vptr 0x008EF268), offset_to_top 0, 2 entries
class ComButton3Lyt : public ::ComButtonLytBase
{
public:
    // (inline in the owners)
    ComButton3Lyt() {}
    // 0x0022C6DC (deleting dtor)
    ~ComButton3Lyt() override; // 0x0022C750 slot 0x00

    // the layout and the buttons on their positions
    bool Setup(); // 0x0022C5FC
    // the buttons leave their panes
    bool Finalize(); // 0x0022C6B4
    // the buttons and their state
    void Update(); // 0x0022C6D4

    Control3Button m_Control; // 0x398
};
ASSERT_SIZE(ComButton3Lyt, 0x398 + sizeof(Control3Button));
