#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "ssys/st/dListNode.h"

// RTTI N8MenuBase4ListE @ 0x008D4028
// vtable 0x0090C0E4 (vptr 0x0090C0EC), offset_to_top 0, 2 entries
class MenuBase::List : public ::ssys::st::ListNode
{
public:
    List(); // ctor address unknown
    virtual ~List(); // 0x006AB408 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x006AB404 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
