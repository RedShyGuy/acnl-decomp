#pragma once

#include "decomp.h"
#include "Letter/dLetterDragWindowBase.h"
#include "state/dMode.h"

// RTTI 20LetterDragPostWindow @ 0x008CCBB4
// vtable 0x008F5894 (vptr 0x008F589C), offset_to_top 0, 8 entries
// vtable 0x008F58BC (vptr 0x008F58C4), offset_to_top -8508, 3 entries
class LetterDragPostWindow : public ::LetterDragWindowBase, public ::state::Mode<LetterDragPostWindow>
{
public:
    LetterDragPostWindow(); // ctor candidate(s) 0x00320100 (unverified)
    virtual void vf_0x08(); // 0x00320024 slot 0x08 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x0C(); // 0x0031FDDC slot 0x0C | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x10(); // 0x00320020 slot 0x10 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x14(); // 0x0031FE00 slot 0x14 | virtual slot, introduced by LetterDragPostWindow
    virtual ~LetterDragPostWindow(); // 0x003201B8 slot 0x18 | slot vf_0x18 of LetterDragPostWindow
    // 0x003201A8 slot 0x1C | slot vf_0x1C of LetterDragPostWindow (deleting dtor)
};
