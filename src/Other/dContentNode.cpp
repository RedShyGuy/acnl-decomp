#include "Other/dContentNode.h"

// 0x001C8764 (name after the RTTI)
ContentNode::ContentNode() : m_pPane(NULL)
{
}

// 0x001C8788 slot 0x00
// 0x001C8784 (deleting dtor)
ContentNode::~ContentNode()
{
    // empty (in the original too)
}
