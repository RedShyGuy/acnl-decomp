#pragma once

#include "decomp.h"
#include "Other/dComButtonLytBase.h"
#include "Other/dControl1Button.h"

// The layout of one common button (class name from the RTTI, all other names are ours).
// RTTI 13ComButton1Lyt @ 0x008CB9B8
// vtable 0x008EF240 (vptr 0x008EF248), offset_to_top 0, 2 entries
class ComButton1Lyt : public ::ComButtonLytBase
{
public:
    // (inline in the owners)
    ComButton1Lyt() {}
    // 0x0022C2C8 (deleting dtor)
    ~ComButton1Lyt() override; // 0x0022C33C slot 0x00

    // the layout and the buttons on their positions
    bool Setup(s32 position); // 0x0022C23C
    // the buttons leave their panes
    bool Finalize(); // 0x0022C2A8
    // the buttons and their state (a nop entry that falls into ControlButtonBase::Update)
    void Update(); // 0x002CD740

    Control1Button m_Control; // 0x398
};
ASSERT_SIZE(ComButton1Lyt, 0x398 + sizeof(Control1Button));
