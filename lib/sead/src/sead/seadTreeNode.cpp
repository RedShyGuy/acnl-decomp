#include "sead/seadTreeNode.h"

namespace sead {
// 0x0012CE74 | nintendogs:bytes [tier A]
sead::TreeNode::TreeNode()
{
    clearLinks();
}

// 0x00561590 | nintendogs:bytes [tier A]
void sead::TreeNode::detachSubTree()
{
    if (mPrev != nullptr) {
        mPrev->mNext = mNext;
        if (mNext != nullptr) {
            mNext->mPrev = mPrev;
            mNext = nullptr;
        }
        mPrev = nullptr;
        mParent = nullptr;
    } else {
        if (mParent != nullptr) {
            mParent->mChild = mNext;
            mParent = nullptr;
        }
        if (mNext != nullptr) {
            mNext->mPrev = mPrev;
            mNext = nullptr;
        }
    }
}

// 0x005615F4 | nintendogs:bytes [tier A]
void sead::TreeNode::pushBackChild(TreeNode* node)
{
    node->detachSubTree();
    if (mChild == nullptr) {
        mChild = node;
        node->mParent = this;
    } else {
        mChild->pushBackSibling_(node);
    }
}

// 0x00561654 | nintendogs:bytes [tier A]
void sead::TreeNode::pushFrontChild(TreeNode* node)
{
    node->detachSubTree();
    if (mChild == nullptr) {
        mChild = node;
        node->mParent = this;
    } else {
        node->mNext = mChild;
        mChild->mPrev = node;
        mChild = node;
        node->mParent = this;
    }
}

// 0x00561694 | nintendogs:bytes [tier A]
void sead::TreeNode::clearChildLinksRecursively_()
{
    TreeNode* child = mChild;
    while (child != nullptr) {
        TreeNode* next = child->mNext;
        child->clearChildLinksRecursively_();
        child->clearLinks();
        child = next;
    }
}

// 0x005616D4 | nintendogs:bytes [tier A]
void sead::TreeNode::detachAll()
{
    detachSubTree();
    clearChildLinksRecursively_();
    clearLinks();
}

} // namespace sead
