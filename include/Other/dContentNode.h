#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "ssys/st/dListNode.h"

namespace nw {
namespace lyt {
class Pane;
}
} // namespace nw

// A pane that scrolls with a CommonScrollBar: its position without scrolling (only the class name is from the RTTI;
// member names are ours).
// RTTI 11ContentNode @ 0x008CB2C0
// vtable 0x008ECB40 (vptr 0x008ECB48), offset_to_top 0, 2 entries
class ContentNode : public ::ssys::st::ListNode
{
public:
    ContentNode(); // 0x001C8764
    virtual ~ContentNode(); // 0x001C8788 slot 0x00
    // 0x001C8784 slot 0x04 (deleting dtor)

    nw::lyt::Pane* m_pPane;          // 0x0C
    nn::math::VEC3 m_BasePosition;   // 0x10 the translation at scroll 0
};
ASSERT_SIZE(ContentNode, 0x1C);
