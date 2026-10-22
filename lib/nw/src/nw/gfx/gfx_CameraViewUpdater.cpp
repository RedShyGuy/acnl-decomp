#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// 0x0097F460
const TypeInfo nw::gfx::CameraViewUpdater::s_TypeInfo(nullptr);
// 0x00AEFF24
const nn::math::VEC3 nw::gfx::CameraViewUpdater::s_DefaultTargetPosition(0.0f, 0.0f, -1.0f);
// 0x00AEFF30
const nn::math::VEC3 nw::gfx::CameraViewUpdater::s_DefaultUpwardVector(0.0f, 1.0f, 0.0f);

// sets s_TypeInfo and the default vectors (and a third vector (0, 0, 0) at 0xAEFF3C that nothing uses)
// 0x0079E84C (static initializer)

} // namespace gfx
} // namespace nw
