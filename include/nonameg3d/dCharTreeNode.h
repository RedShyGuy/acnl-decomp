#pragma once

#include "decomp.h"
#include "nonameg3d/dChar.h"
#include "sead/seadTreeMapNode.h"

namespace nonameg3d {
// a node of a tree map of names: the name and its value (names are ours)
// RTTI N9nonameg3d12CharTreeNodeE @ 0x008D4238
// vtable 0x0090C660 (vptr 0x0090C668), offset_to_top 0, 3 entries
class CharTreeNode : public ::sead::TreeMapNode<nonameg3d::Char>
{
public:
    // (inline)
    CharTreeNode() : m_pValue(NULL) {}
    virtual ~CharTreeNode(); // 0x0070B274 slot 0x00
    // 0x0070B270 (deleting dtor)
    void erase_() override; // 0x0070B26C slot 0x08

    void* m_pValue; // 0x18
};
ASSERT_SIZE(CharTreeNode, 0x1C);
} // namespace nonameg3d
