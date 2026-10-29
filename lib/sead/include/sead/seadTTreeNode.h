#pragma once

#include "decomp.h"
#include "sead/seadTreeNode.h"

namespace sead {
// Instantiations found in the binary:
//   sead::TTreeNode<sead::MethodTreeNode*>  typeinfo 0x008D23B8
//   sead::TTreeNode<sead::TaskBase*>  typeinfo 0x008D23C4
//
// A TreeNode with the object it stands for (member name and accessors are ours).
template <typename T>
class TTreeNode : public TreeNode
{
public:
    TTreeNode() : TreeNode(), mData() {}
    explicit TTreeNode(T data) : TreeNode(), mData(data) {}

    // the objects of the linked nodes (NULL without one)
    TTreeNode* child() const { return static_cast<TTreeNode*>(mChild); }
    TTreeNode* next() const { return static_cast<TTreeNode*>(mNext); }
    TTreeNode* parent() const { return static_cast<TTreeNode*>(mParent); }
    T val() const { return mData; }

    T mData; // 0x10
};
} // namespace sead
