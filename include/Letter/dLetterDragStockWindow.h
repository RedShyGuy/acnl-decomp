#pragma once

#include "decomp.h"
#include "Letter/dLetterDragWindowBase.h"
#include "state/dMode.h"

// RTTI 21LetterDragStockWindow @ 0x008CCCF8
// vtable 0x008F6358 (vptr 0x008F6360), offset_to_top 0, 7 entries
// vtable 0x008F637C (vptr 0x008F6384), offset_to_top -8508, 3 entries
class LetterDragStockWindow : public ::LetterDragWindowBase, public ::state::Mode<LetterDragStockWindow>
{
public:
    LetterDragStockWindow(); // ctor address unknown
    virtual void vf_0x04(); // 0x00328C7C slot 0x04 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x08(); // 0x00328DE4 slot 0x08 | virtual slot, introduced by LetterDragWindowBase
    virtual void vf_0x0C(); // 0x00328C5C slot 0x0C | virtual slot, introduced by LetterDragWindowBase
    virtual ~LetterDragStockWindow(); // 0x00328E90 slot 0x14 | slot vf_0x14 of LetterDragStockWindow
    // 0x00328E80 slot 0x18 | slot vf_0x18 of LetterDragStockWindow (deleting dtor)
};
