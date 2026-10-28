#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead8TreeNodeE @ 0x008D2238
//
// A node of a tree: the parent, the first child and the siblings before and after the node. The
// class and function names are from the reference symbols, the member names and the inline helpers
// are ours.
class TreeNode
{
public:
    TreeNode(); // 0x0012CE74 | nintendogs:bytes [tier A]

    // takes the node (with its children) out of the tree
    void detachSubTree(); // 0x00561590 | nintendogs:bytes [tier A]
    void pushBackChild(TreeNode* node); // 0x005615F4 | nintendogs:bytes [tier A]
    void pushFrontChild(TreeNode* node); // 0x00561654 | nintendogs:bytes [tier A]
    // unlinks all nodes below this one
    void clearChildLinksRecursively_(); // 0x00561694 | nintendogs:bytes [tier A]
    // takes the node out of the tree and unlinks all nodes below it
    void detachAll(); // 0x005616D4 | nintendogs:bytes [tier A]

    void clearLinks() { mParent = mChild = mNext = mPrev = nullptr; }
    // links node behind the last sibling of this node
    void pushBackSibling_(TreeNode* node)
    {
        node->detachSubTree();
        TreeNode* last = this;
        while (last->mNext != nullptr) {
            last = last->mNext;
        }
        last->mNext = node;
        node->mPrev = last;
        node->mParent = last->mParent;
    }

    TreeNode* mParent; // 0x0
    TreeNode* mChild;  // 0x4, the first child
    TreeNode* mNext;   // 0x8
    TreeNode* mPrev;   // 0xC
};
ASSERT_SIZE(TreeNode, 0x10);
} // namespace sead
