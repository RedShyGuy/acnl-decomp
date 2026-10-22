#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Vector2.h"

namespace nw {
namespace lyt {
class GraphicsResource;
class Layout;

// RTTI N2nw3lyt8DrawInfoE @ 0x008D08C0
// vtable 0x00902E98 (vptr 0x00902EA0), offset_to_top 0, 2 entries
//
// The state of a layout drawing: the matrices, the alpha of the parents and the graphics
// resource. Member names are ours.
class DrawInfo
{
public:
    struct Flag
    {
        u32 isInfluencedAlpha : 1;          // the global alpha applies (a parent influences it)
        u32 isLocationAdjust : 1;           // panes with FLAG_LOCATION_ADJUST are scaled
        u32 isInvisiblePaneCalculateMtx : 1; // the matrices of invisible panes are calculated
        u32 reserve : 29;
    };

    DrawInfo(); // 0x00132304
    virtual ~DrawInfo(); // 0x004BC8FC slot 0x00 (name is ours)
    // 0x004BC8F8 (deleting dtor)

    nn::math::MTX44 m_ProjMtx;                  // 0x04
    nn::math::MTX34 m_ViewMtx;                  // 0x44
    nn::math::VEC2 m_LocationAdjustScale;       // 0x74
    f32 m_GlobalAlpha;                          // 0x7C
    GraphicsResource* m_pGraphicsResource;      // 0x80
    Layout* m_pLayout;                          // 0x84 the layout being drawn
    Flag m_Flag;                                // 0x88
};
ASSERT_OFFSET(DrawInfo, m_ViewMtx, 0x44);
ASSERT_OFFSET(DrawInfo, m_pGraphicsResource, 0x80);
ASSERT_SIZE(DrawInfo, 0x8C);
} // namespace lyt
} // namespace nw
