#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 14PostDragWindow @ 0x008CBD7C
// vtable 0x008F0708 (vptr 0x008F0710), offset_to_top 0, 3 entries
class PostDragWindow : public ::state::Mode<PostDragWindow>
{
public:
    PostDragWindow(); // ctor address unknown
    virtual ~PostDragWindow(); // 0x00274BC8 slot 0x00 | slot vf_0x00 of PostDragWindow
    // 0x00274BB8 slot 0x04 | slot vf_0x04 of PostDragWindow (deleting dtor)
    virtual void vf_0x08(); // 0x0082B508 slot 0x08 | virtual slot, introduced by PostDragWindow
};
