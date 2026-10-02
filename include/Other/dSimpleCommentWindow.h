#pragma once

#include "decomp.h"
#include "Other/dExplainWindow.h"

// RTTI 19SimpleCommentWindow @ 0x008CCA74
// vtable 0x008F5334 (vptr 0x008F533C), offset_to_top 0, 13 entries
class SimpleCommentWindow : public ::ExplainWindow
{
public:
    SimpleCommentWindow(); // ctor address unknown
    virtual ~SimpleCommentWindow(); // 0x002FEE30 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x002FECA0 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x10(); // 0x002FEC4C slot 0x10 | virtual slot, introduced by InOutWindow
    virtual void vf_0x18(); // 0x002FEC78 slot 0x18 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x002FE984 slot 0x28 | virtual slot, introduced by InOutWindow
    virtual void vf_0x2C(); // 0x002FE530 slot 0x2C | virtual slot, introduced by InOutWindow
};
