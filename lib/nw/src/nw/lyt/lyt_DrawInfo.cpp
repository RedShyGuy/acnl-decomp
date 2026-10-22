#include "nw/lyt/lyt_DrawInfo.h"

#include "nn/math/math_ARMv6.h"

namespace nw {
namespace lyt {
// 0x00132304
nw::lyt::DrawInfo::DrawInfo()
    : m_ViewMtx(nn::math::MTX34::Identity()), m_LocationAdjustScale(1.0f, 1.0f), m_GlobalAlpha(1.0f), m_pGraphicsResource(nullptr),
      m_pLayout(nullptr), m_Flag()
{
}

// 0x004BC8FC slot 0x00 (name is ours)
// 0x004BC8F8 (deleting dtor)
nw::lyt::DrawInfo::~DrawInfo()
{
    // empty (in the original too)
}

} // namespace lyt
} // namespace nw
