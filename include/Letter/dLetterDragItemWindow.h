#pragma once

#include "decomp.h"
#include "Letter/dLetterDragWindowBase.h"
#include "state/dMode.h"

// RTTI 20LetterDragItemWindow @ 0x008CCB94
// vtable 0x008F585C (vptr 0x008F5864), offset_to_top 0, 7 entries
// vtable 0x008F5880 (vptr 0x008F5888), offset_to_top -8508, 3 entries
class LetterDragItemWindow : public ::LetterDragWindowBase, public ::state::Mode<LetterDragItemWindow>
{
public:
    LetterDragItemWindow(); // ctor candidate(s) 0x0031FA6C (unverified)
    virtual void vf_0x04(); // 0x0031F590 slot 0x04 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x08(); // 0x0031F7B0 slot 0x08 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x10(); // 0x00320A18 slot 0x10 | virtual slot, introduced by LetterDragWindowBase
    virtual ~LetterDragItemWindow(); // 0x0031FB48 slot 0x14 | slot vf_0x14 of LetterDragItemWindow
    // 0x0031FB38 slot 0x18 | slot vf_0x18 of LetterDragItemWindow (deleting dtor)
};
