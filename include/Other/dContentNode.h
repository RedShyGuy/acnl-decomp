#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

// RTTI 11ContentNode @ 0x008CB2C0
// vtable 0x008ECB40 (vptr 0x008ECB48), offset_to_top 0, 2 entries
class ContentNode : public ::ssys::st::ListNode
{
public:
    ContentNode(); // ctor candidate(s) 0x001C8764 (unverified)
    virtual ~ContentNode(); // 0x001C8788 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x001C8784 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
