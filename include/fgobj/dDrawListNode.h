#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace fgobj {
class ObjectBase;

// The node of an object in the DrawList (class name from the RTTI, the member is ours).
// RTTI N5fgobj12DrawListNodeE @ 0x008D29C4
// vtable 0x00908C84 (vptr 0x00908C8C), offset_to_top 0, 2 entries
class DrawListNode : public ::ssys::st::ListNode
{
public:
    DrawListNode() {}
    virtual ~DrawListNode(); // 0x00594EDC slot 0x00 | slot vf_0x00 of ssys::st::ListNode

    /* 0xC */ ObjectBase* m_pObject;
};
ASSERT_SIZE(DrawListNode, 0x10);
} // namespace fgobj
