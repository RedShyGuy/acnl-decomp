#pragma once

#include "decomp.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt9LayoutMgrE @ 0x008D24C8
// vtable 0x00907358 (vptr 0x00907360), offset_to_top 0, 2 entries
class LayoutMgr
{
public:
    LayoutMgr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0056AC34 slot 0x00 | virtual slot, introduced by ssys::ma::lyt::LayoutMgr
    virtual void vf_0x04(); // 0x0056AC04 slot 0x04 | virtual slot, introduced by ssys::ma::lyt::LayoutMgr
    void Get(); // 0x00133A78 | libgarden [tier A]
    void Register(ssys::ma::lyt::Base2D*, bool); // 0x0056A954 | libgarden [tier A]
    void DrawBegin(unsigned long, unsigned long); // 0x0056A9A4 | libgarden [tier A]
};
} // namespace lyt
} // namespace ma
} // namespace ssys
