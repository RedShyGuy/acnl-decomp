#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

// RTTI 20SoundDataLoadRequest @ 0x008CCC0C
// vtable 0x008F5B2C (vptr 0x008F5B34), offset_to_top 0, 2 entries
class SoundDataLoadRequest : public ::ssys::st::ListNode
{
public:
    SoundDataLoadRequest(); // ctor candidate(s) 0x00128EEC (unverified)
    virtual ~SoundDataLoadRequest(); // 0x0013E4E0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x003220C4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
