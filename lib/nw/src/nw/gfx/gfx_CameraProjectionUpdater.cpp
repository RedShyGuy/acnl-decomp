#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// 0x0097F454
const TypeInfo nw::gfx::CameraProjectionUpdater::s_TypeInfo(nullptr);
// 0x0097F458
const nn::math::VEC2 nw::gfx::CameraProjectionUpdater::s_DefaultCenter(0.0f, 0.0f);

// sets s_TypeInfo and s_DefaultCenter
// 0x0079FCB0 (static initializer)

// 0x008B4BF4
const f32 nw::gfx::CameraProjectionUpdater::s_DefaultNear = 1.0f;
// 0x008B4BF8
const f32 nw::gfx::CameraProjectionUpdater::s_DefaultFar = 1.0f;
// 0x008B4BFC
const f32 nw::gfx::CameraProjectionUpdater::s_DefaultFovy = 0.7853982f;
// 0x008B4C00
const f32 nw::gfx::CameraProjectionUpdater::s_DefaultAspectRatio = 1.0f;
// 0x008B4C04
const f32 nw::gfx::CameraProjectionUpdater::s_DefaultHeight = 1.0f;

} // namespace gfx
} // namespace nw
