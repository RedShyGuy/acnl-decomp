#include "nw/snd/internal/snd_CurveLfoParam.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C72BC | fefates:bytes [tier B]
void nw::snd::internal::CurveLfoParam::Initialize()
{
    depth = 0.0f;
    speed = 6.25f;
    delay = 0;
    range = 1;
    curve = 0;
    phase = 0;
}

} // namespace internal
} // namespace snd
} // namespace nw
