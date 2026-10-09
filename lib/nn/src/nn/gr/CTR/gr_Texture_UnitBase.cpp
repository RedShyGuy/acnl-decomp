#include "nn/gr/CTR/gr_Texture_UnitBase.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x0034B1F4 | fefates:bytes [tier B]
nn::gr::CTR::Texture::UnitBase::UnitBase()
    : m_PhysicalAddr(0), m_Width(0), m_Height(0), m_Format(12), m_WrapT(2), m_WrapS(2), m_MagFilter(0),
      m_MinFilter(0), m_LodBias(0.0f), m_MinLodLevel(0), m_MaxLodLevel(0), m_BorderColorR(0), m_BorderColorG(0),
      m_BorderColorB(0), m_BorderColorA(0)
{
}

} // namespace CTR
} // namespace gr
} // namespace nn
