#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace lyt {
namespace internal {
// The sets of texture coordinates of a picture or the content of a window (a quad each, from
// the layout memory). Member names are ours.
class TexCoordAry
{
public:
    TexCoordAry(); // 0x004BEBF8 | nintendogs:bytes [tier A]

    // the quads are released
    void Free(); // 0x004BE988 | nintendogs:bytes [tier A]
    // room for num quads (the quads are lost when there was less)
    void Reserve(u8 num); // 0x004BE9B4 | libgarden [tier A]
    // num quads: the new ones are the whole texture
    void SetSize(u8 num); // 0x004BEA34 | nintendogs:bytes-fuzzy [tier A]
    void SetCoord(u32 idx, const nn::math::VEC2* coords); // 0x004BEB80 | mk7dlp:bytes [tier B]
    // the quads of a resource (res::Picture / res::WindowContent)
    void Copy(const void* pResTexCoord, u8 texCoordNum); // 0x004BE8E8 | nintendogs:bytes [tier A]

    // (inline)
    u8 GetSize() const { return m_Num; }
    const TexCoordQuad* GetArray() const { return m_pData; }

    u8 m_Cap;               // 0x0
    u8 m_Num;               // 0x1
    TexCoordQuad* m_pData;  // 0x4
};
ASSERT_SIZE(TexCoordAry, 0x8);
} // namespace internal
} // namespace lyt
} // namespace nw
