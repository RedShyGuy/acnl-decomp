#include "graphic/dScreenCamera.h"

namespace graphic {
namespace {
// the draw mode of the top screen (the left image; ssys::ma::lyt::Layout)
const u32 DRAW_MODE_TOP_LEFT = 0x400;
} // namespace

// 0x007132D8 (name is ours)
const nn::math::MTX34& graphic::ScreenCamera::GetViewMatrix(u32 drawMode) const
{
    return drawMode == DRAW_MODE_TOP_LEFT ? m_LeftMatrices.view : m_RightMatrices.view;
}

// 0x00713318 (name is ours)
const nn::math::MTX44& graphic::ScreenCamera::GetProjectionMatrix(u32 drawMode) const
{
    return drawMode == DRAW_MODE_TOP_LEFT ? m_LeftMatrices.projection : m_RightMatrices.projection;
}

// 0x00713328 (name is ours)
const nn::math::MTX34& graphic::ScreenCamera::GetInverseViewMatrix(u32 drawMode) const
{
    return drawMode == DRAW_MODE_TOP_LEFT ? m_LeftMatrices.inverseView : m_RightMatrices.inverseView;
}

} // namespace graphic
