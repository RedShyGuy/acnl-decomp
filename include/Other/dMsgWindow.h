#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 9MsgWindow @ 0x008CD7A8
// vtable 0x008FA6C0 (vptr 0x008FA6C8), offset_to_top 0, 3 entries
class MsgWindow : public ::state::Mode<MsgWindow>
{
public:
    MsgWindow(); // ctor address unknown
    virtual void vf_0x00(); // 0x006E7754 slot 0x00 | virtual slot, introduced by MsgWindow
    virtual void vf_0x04(); // 0x006E76A8 slot 0x04 | virtual slot, introduced by MsgWindow
    virtual void vf_0x08(); // 0x0082D7B8 slot 0x08 | virtual slot, introduced by MsgWindow
};
