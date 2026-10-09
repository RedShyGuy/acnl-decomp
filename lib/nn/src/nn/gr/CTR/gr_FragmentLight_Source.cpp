#include "nn/gr/CTR/gr_FragmentLight_Source.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
namespace {
// an 8 bit color in the 10 bit fields of a lighting color register
inline u32 LightColor(u8 r, u8 g, u8 b)
{
    return (g << 10) | (r << 20) | b;
}
} // namespace

// 0x00349A80 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nn::gr::CTR::FragmentLight::Source::Source()
    : m_Id(0), m_IsTwoSideDiffuse(false), m_IsGeometricFactor0(false), m_IsGeometricFactor1(false),
      m_DiffuseR(0xFF), m_DiffuseG(0xFF), m_DiffuseB(0xFF), m_AmbientR(0), m_AmbientG(0), m_AmbientB(0),
      m_Specular0R(0xFF), m_Specular0G(0xFF), m_Specular0B(0xFF), m_Specular1R(0xFF), m_Specular1G(0xFF),
      m_Specular1B(0xFF), m_PositionXY(0), m_PositionZ(0), m_IsDirectional(0), m_DistanceAttenuationBias(0),
      m_DistanceAttenuationScale(0), m_SpotDirectionXY(0), m_SpotDirectionZ(0)
{
}

// 0x007275FC | nintendogs:bytes [confirmed by fefates] [tier A]
u32* nn::gr::CTR::FragmentLight::Source::MakeAllCommand(u32* command) const
{
    *command++ = LightColor(m_Specular0R, m_Specular0G, m_Specular0B);
    *command++ = detail::CommandHeader(0x140 + m_Id * 16, 0xF, 11, true);
    *command++ = LightColor(m_Specular1R, m_Specular1G, m_Specular1B);
    *command++ = LightColor(m_DiffuseR, m_DiffuseG, m_DiffuseB);
    *command++ = LightColor(m_AmbientR, m_AmbientG, m_AmbientB);
    *command++ = m_PositionXY;
    *command++ = m_PositionZ;
    *command++ = m_SpotDirectionXY;
    *command++ = m_SpotDirectionZ;
    *command++ = 0;
    *command++ = m_IsDirectional | ((m_IsTwoSideDiffuse ? 1 : 0) << 1) | ((m_IsGeometricFactor0 ? 1 : 0) << 2)
        | ((m_IsGeometricFactor1 ? 1 : 0) << 3);
    *command++ = m_DistanceAttenuationBias;
    *command++ = m_DistanceAttenuationScale;
    *command++ = 0;
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
