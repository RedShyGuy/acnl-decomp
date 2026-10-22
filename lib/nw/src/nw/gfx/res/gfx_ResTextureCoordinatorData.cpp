#include "nw/gfx/res/gfx_ResTextureCoordinatorData.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A8FCC | mk7dlp:bytes [tier B]
nw::gfx::res::ResTextureCoordinatorData::ResTextureCoordinatorData(const ResTextureCoordinatorData& other)
    : m_Unknown0x0(other.m_Unknown0x0), mappingMethod(other.mappingMethod), m_Unknown0x8(other.m_Unknown0x8),
      m_Unknown0xC(other.m_Unknown0xC), m_Unknown0x10(other.m_Unknown0x10), m_Unknown0x18(other.m_Unknown0x18),
      m_Unknown0x1C(other.m_Unknown0x1C), m_Unknown0x24(other.m_Unknown0x24), isEnabled(other.isEnabled),
      m_Unknown0x26(other.m_Unknown0x26)
{
    nn::math::ARMv6::MTX34CopyAsm(&textureMatrix, &other.textureMatrix);
}

} // namespace res
} // namespace gfx
} // namespace nw
