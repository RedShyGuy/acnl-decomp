#pragma once

#include "decomp.h"
#include "Other/dLayout.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

// RTTI 8WaitIcon @ 0x008CD5B4
// vtable 0x008F9F78 (vptr 0x008F9F80), offset_to_top 0, 3 entries
//
// The wait icon of the message window (a layout with three animations). Only the layout is known (from the
// constructor of MsgWindow and the destructor); member names are ours.
class WaitIcon : public ::state::Mode<WaitIcon>
{
public:
    // (inline)
    WaitIcon() : m_Unknown0x430(0), m_Unknown0x431(0), m_Sound(0), m_Unknown0x438(0.0f) {}
    virtual ~WaitIcon(); // 0x006C9CA4 slot 0x00
    // 0x006C9C64 (deleting dtor)
    virtual const char* GetName(Func func) const; // 0x0082D470 slot 0x08

    Layout m_Layout;                                  // 0x024
    ssys::ma::lyt::ArcResAccReader m_ResAccReader;    // 0x170
    ssys::ma::lyt::Animation m_Animation0;            // 0x3B8
    ssys::ma::lyt::Animation m_Animation1;            // 0x3E0
    ssys::ma::lyt::Animation m_Animation2;            // 0x408
    u8 m_Unknown0x430;                                // 0x430
    u8 m_Unknown0x431;                                // 0x431
    u32 m_Sound;                                      // 0x434 SeID (TalkSystem sets it by the kind of the wait)
    f32 m_Unknown0x438;                               // 0x438
};
ASSERT_SIZE(WaitIcon, 0x43C);
