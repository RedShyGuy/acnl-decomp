#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

// RTTI 10ListNodeId @ 0x008CB108
class ListNodeId : public ::ssys::st::ListNode
{
public:
    ListNodeId(); // ctor address unknown

    u32 m_Id; // 0x0C (name is ours)
};
