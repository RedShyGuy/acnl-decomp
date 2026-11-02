#include "nonameg3d/dCharTreeNode.h"

namespace nonameg3d {
// 0x0070B26C slot 0x08 (name is ours)
void nonameg3d::CharTreeNode::erase_()
{
    // empty (in the original too)
}

// 0x0070B274 slot 0x00
// 0x0070B270 (deleting dtor)
nonameg3d::CharTreeNode::~CharTreeNode()
{
    // empty (in the original too)
}

} // namespace nonameg3d

// 0x00811698 (name is ours)
template sead::TreeMapNode<nonameg3d::Char>* sead::TreeMapImpl<nonameg3d::Char>::insert(sead::TreeMapNode<nonameg3d::Char>* h,
                                                                                    sead::TreeMapNode<nonameg3d::Char>* node);
