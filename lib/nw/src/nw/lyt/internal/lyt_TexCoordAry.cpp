#include "nw/lyt/internal/lyt_TexCoordAry.h"

#include "nw/lyt/lyt_Layout.h"

namespace nw {
namespace lyt {
namespace internal {
// 0x004BEBF8 | nintendogs:bytes [tier A]
nw::lyt::internal::TexCoordAry::TexCoordAry() : m_Cap(0), m_Num(0), m_pData(nullptr)
{
}

// 0x004BE988 | nintendogs:bytes [tier A]
void nw::lyt::internal::TexCoordAry::Free()
{
    if (m_pData != nullptr) {
        Layout::FreeMemory(m_pData);
        m_pData = nullptr;
        m_Cap = 0;
        m_Num = 0;
    }
}

// 0x004BE9B4 | libgarden [tier A]
void nw::lyt::internal::TexCoordAry::Reserve(u8 num)
{
    if (m_Cap < num) {
        Free();
        m_pData = reinterpret_cast<TexCoordQuad*>(Layout::NewArray<nn::math::VEC2>(num * 4));
        if (m_pData != nullptr) {
            m_Cap = num;
        }
    }
}

// 0x004BEA34 | nintendogs:bytes-fuzzy [tier A]
void nw::lyt::internal::TexCoordAry::SetSize(u8 num)
{
    if (m_pData != nullptr && num <= m_Cap) {
        // 0x00AEFF48
        static const TexCoordQuad texCoords = {
            nn::math::VEC2(0.0f, 0.0f),
            nn::math::VEC2(1.0f, 0.0f),
            nn::math::VEC2(0.0f, 1.0f),
            nn::math::VEC2(1.0f, 1.0f),
        };
        for (int j = 0; j < 4; ++j) {
            for (int i = m_Num; i < num; ++i) {
                m_pData[i][j] = texCoords[j];
            }
        }
        m_Num = num;
    }
}

// 0x004BEB80 | mk7dlp:bytes [tier B]
void nw::lyt::internal::TexCoordAry::SetCoord(u32 idx, const nn::math::VEC2* coords)
{
    for (int i = 0; i < 4; ++i) {
        m_pData[idx][i] = coords[i];
    }
}

// 0x004BE8E8 | nintendogs:bytes [tier A]
void nw::lyt::internal::TexCoordAry::Copy(const void* pResTexCoord, u8 texCoordNum)
{
    m_Num = (m_Num < texCoordNum) ? texCoordNum : m_Num;
    const TexCoordQuad* src = static_cast<const TexCoordQuad*>(pResTexCoord);
    for (int j = 0; j < texCoordNum; ++j) {
        for (int i = 0; i < 4; ++i) {
            m_pData[j][i] = src[j][i];
        }
    }
}

} // namespace internal
} // namespace lyt
} // namespace nw
