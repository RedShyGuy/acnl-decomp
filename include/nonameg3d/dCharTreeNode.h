#pragma once

#include "decomp.h"
#include "nonameg3d/dChar.h"
#include "sead/seadTreeMapNode.h"

namespace nonameg3d {
// RTTI N9nonameg3d12CharTreeNodeE @ 0x008D4238
// vtable 0x0090C660 (vptr 0x0090C668), offset_to_top 0, 3 entries
class CharTreeNode : public ::sead::TreeMapNode<nonameg3d::Char>
{
public:
    CharTreeNode(); // ctor address unknown
    virtual void vf_0x00(); // 0x0070B274 slot 0x00 | virtual slot, introduced by nonameg3d::CharTreeNode
    virtual void vf_0x04(); // 0x0070B270 slot 0x04 | virtual slot, introduced by nonameg3d::CharTreeNode
    virtual void vf_0x08(); // 0x0070B26C slot 0x08 | virtual slot, introduced by nonameg3d::CharTreeNode
};
} // namespace nonameg3d
