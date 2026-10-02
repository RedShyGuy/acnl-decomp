#include "ssys/ma/lyt/dFrameCtrl.h"
#include "ssys/ma/lyt/dAnimation.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x0056A374 slot 0x00 | libgarden
ssys::ma::lyt::Animation::~Animation()
{
}

// 0x0056A320 slot 0x04 | virtual slot, introduced by ssys::ma::lyt::Animation
void ssys::ma::lyt::Animation::vf_0x04()
{
}

// 0x0056A2C8 slot 0x08 | virtual slot, introduced by ssys::ma::lyt::Animation
void ssys::ma::lyt::Animation::vf_0x08()
{
}

// 0x00126200 | libgarden [tier A]
ssys::ma::lyt::Animation::Animation()
{
}

// 0x0056A02C | libgarden [tier A]
void ssys::ma::lyt::Animation::IncreaseFrame()
{
}

// 0x0056A0CC | libgarden [tier A]
void ssys::ma::lyt::Animation::Initialize(char const*, ssys::ma::lyt::ResAccInterface const*)
{
}

// 0x00750DEC | libgarden [tier A]
void ssys::ma::lyt::Animation::IsDone() const
{
}

} // namespace lyt
} // namespace ma
} // namespace ssys
