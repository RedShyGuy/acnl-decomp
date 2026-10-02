#pragma once

#include "decomp.h"
#include "net/dSender.h"
#include "ssys/st/dListNode.h"

// RTTI N3net6Sender6HandleE @ 0x008D0FF0
// vtable 0x009046CC (vptr 0x009046D4), offset_to_top 0, 2 entries
class net::Sender::Handle : public ::ssys::st::ListNode
{
public:
    Handle(); // ctor candidate(s) 0x0051B248 (unverified)
    virtual ~Handle(); // 0x0051B2A0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0051B290 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
