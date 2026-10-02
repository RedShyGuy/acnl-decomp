#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// RTTI 18MyButtonActionNode @ 0x008CC874
// vtable 0x008F4910 (vptr 0x008F4918), offset_to_top 0, 16 entries
class MyButtonActionNode : public ::ButtonActionNode
{
public:
    MyButtonActionNode(); // ctor candidate(s) 0x002E5E84 (unverified)
    virtual ~MyButtonActionNode(); // 0x002B74C0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002E5E9C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void ResetSelect(); // 0x002E5E6C slot 0x0C | slot vf_0x0C of ButtonActionNode
};
