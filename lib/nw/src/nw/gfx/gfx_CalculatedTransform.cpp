#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace gfx {
// 0x0049FBBC | nintendogs:bytes [tier B]
void nw::gfx::CalculatedTransform::SetRotateXYZ(float, float, float)
{
}

// 0x0049FDE8 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::SetTransform(const nn::math::Transform3&)
{
}

// 0x004A0020 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateRotateFlags()
{
}

// 0x004A0120 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateCompositeFlags()
{
}

// 0x004A0154 | nintendogs:bytes [tier A]
void nw::gfx::CalculatedTransform::UpdateTranslateFlags()
{
}

// 0x004A01B0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::CalculatedTransform::NormalizeRotateMatrix()
{
}

// 0x004A02E8 | nintendogs:callgraph [tier A]
void nw::gfx::CalculatedTransform::SetRotateAndTranslate(const nn::math::VEC3&, const nn::math::VEC3&)
{
}

} // namespace gfx
} // namespace nw
