#include "nn/gr/CTR/gr_Vertex_LoadArray.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x0034B0AC | mk7dlp:callgraph [confirmed by fefates] [tier A]
void nn::gr::CTR::Vertex::LoadArray::DisableAll()
{
    m_PhysicalAddr = 0;
    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        m_Types[i] = static_cast<PicaDataVertexAttrType>(0);
        m_ByteSizes[i] = 0;
        m_AttrIndex[i] = -1;
    }
}

// 0x0034B0E8 | fefates:bytes [tier B]
nn::gr::CTR::Vertex::LoadArray::LoadArray() : m_PhysicalAddr(0)
{
}

} // namespace CTR
} // namespace gr
} // namespace nn
