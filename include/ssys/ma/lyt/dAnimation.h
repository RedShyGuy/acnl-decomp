#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dFrameCtrl.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt9AnimationE @ 0x008D24A8
// vtable 0x00907334 (vptr 0x0090733C), offset_to_top 0, 3 entries
class Animation : public ::ssys::ma::lyt::FrameCtrl
{
public:
    virtual ~Animation(); // 0x0056A374 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x0056A320 slot 0x04 | virtual slot, introduced by ssys::ma::lyt::Animation
    virtual void vf_0x08(); // 0x0056A2C8 slot 0x08 | virtual slot, introduced by ssys::ma::lyt::Animation
    Animation(); // 0x00126200 | libgarden [tier A]
    void IncreaseFrame(); // 0x0056A02C | libgarden [tier A]
    void Initialize(char const*, ssys::ma::lyt::ResAccInterface const*); // 0x0056A0CC | libgarden [tier A]
    void IsDone() const; // 0x00750DEC | libgarden [tier A]
};
} // namespace lyt
} // namespace ma
} // namespace ssys
