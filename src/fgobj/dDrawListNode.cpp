#include "ssys/st/dListNode.h"
#include "fgobj/dDrawListNode.h"

namespace fgobj {
// 0x00594EDC slot 0x00 | slot vf_0x00 of ssys::st::ListNode
// 0x00594ED8 (deleting dtor)
fgobj::DrawListNode::~DrawListNode()
{
    // empty (in the original too)
}

} // namespace fgobj
