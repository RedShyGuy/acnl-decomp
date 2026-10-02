#pragma once

#include "decomp.h"

namespace lyt {
// RTTI N3lyt6ObjectE @ 0x008D0F88
// vtable 0x00904544 (vptr 0x0090454C), offset_to_top 0, 4 entries
class Object
{
public:
    Object(); // ctor candidate(s) 0x0050A2DC (unverified)
    virtual ~Object(); // 0x0050A3B0 slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x0050A39C slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x08(); // 0x00509D40 slot 0x08 | virtual slot, introduced by lyt::Object
    virtual void vf_0x0C(); // 0x00509C38 slot 0x0C | virtual slot, introduced by lyt::Object
};
} // namespace lyt
