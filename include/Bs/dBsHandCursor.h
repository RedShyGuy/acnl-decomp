#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"

// RTTI 12BsHandCursor @ 0x008CB52C
// vtable 0x008EE0A4 (vptr 0x008EE0AC), offset_to_top 0, 16 entries
// vtable 0x008EE0EC (vptr 0x008EE0F4), offset_to_top -20, 3 entries
class BsHandCursor : public ::Base, public ::state::Mode<BsHandCursor>
{
public:
    BsHandCursor(); // ctor candidate(s) 0x007ED4BC (unverified)
    virtual ~BsHandCursor(); // 0x001FACA0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FAC3C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x001FA5D8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x001FAC1C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x001FA9D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x001FA5AC slot 0x30 | slot vf_0x30 of oml::framework::Process
    void Disable(); // 0x001FA910 | libgarden [tier A]
    // (names are ours) shows the cursor for the cursor keys / circle pad
    void Enable(); // 0x001FA84C
    void SetPressed(bool pressed); // 0x001F9E40
    // moves the cursor pane (and animates its layout)
    void SetPosition(const nn::math::VEC3* pos); // 0x001F9FA0
    // (purpose not known yet; called when a scroll bar thumb is grabbed / let go with the cursor)
    void Func1F9FC4(); // 0x001F9FC4
    void Func1FA0C4(bool value); // 0x001FA0C4

    u8 _unknown38[0x4C0 - 0x38];  // 0x038 (the layout of the cursor at 0x38)
    nn::math::VEC2 m_Position;    // 0x4C0 (name is ours)
    nn::math::VEC2 m_Move;        // 0x4C8 non-zero while the cursor moves (name is ours)
    bool m_Unknown0x4D0;          // 0x4D0
    bool m_IsEnabled;             // 0x4D1 (name is ours)
    u8 m_Unknown0x4D2[5];         // 0x4D2
    bool m_Unknown0x4D7;          // 0x4D7 (set while a scroll bar is moved with the cursor keys)
};
ASSERT_OFFSET(BsHandCursor, m_Position, 0x4C0);
