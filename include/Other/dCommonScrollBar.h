#pragma once

#include "decomp.h"

// RTTI 15CommonScrollBar @ 0x008CC018
// vtable 0x008F18B0 (vptr 0x008F18B8), offset_to_top 0, 7 entries
class CommonScrollBar
{
public:
    CommonScrollBar(); // ctor candidate(s) 0x0029A0EC (unverified)
    virtual ~CommonScrollBar(); // 0x0029A2C4 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x0029A254 slot 0x04 | virtual slot, introduced by CommonScrollBar
    virtual void vf_0x08(); // 0x002991CC slot 0x08 | virtual slot, introduced by CommonScrollBar
    virtual void vf_0x0C(); // 0x00298340 slot 0x0C | virtual slot, introduced by CommonScrollBar
    virtual void vf_0x10(); // 0x0029A080 slot 0x10 | virtual slot, introduced by CommonScrollBar
    virtual void vf_0x14(); // 0x00298E08 slot 0x14 | virtual slot, introduced by CommonScrollBar
    virtual void vf_0x18(); // 0x00298198 slot 0x18 | virtual slot, introduced by CommonScrollBar
    void Finalize(); // 0x00133ACC | libgarden [tier A]
    void SetParent(nw::lyt::Pane*); // 0x0029811C | libgarden [tier A]
    void Initialize(ScrollBarDescription&&); // 0x00299A14 | libgarden [tier A]
    void Process(); // 0x00299D38 | libgarden [tier A]
    CommonScrollBar(void*); // 0x0029A0EC | libgarden [tier A]
};
