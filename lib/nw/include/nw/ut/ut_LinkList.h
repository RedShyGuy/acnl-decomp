#pragma once

#include "decomp.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace ut {
// An intrusive list of T: the LinkListNode of an object is TNodeOffset bytes into it. The class
// name and the template arguments are from the reference symbols (e.g. LinkList<HeapBase, 4>).
template <typename T, long TNodeOffset>
class LinkList : public internal::LinkListImpl
{
public:
    // the object of a node / the node of an object
    static T* GetPointerFromNode(LinkListNode* pNode)
    {
        return reinterpret_cast<T*>(reinterpret_cast<uptr>(pNode) - TNodeOffset);
    }
    static LinkListNode* GetNodeFromPointer(T* p)
    {
        return reinterpret_cast<LinkListNode*>(reinterpret_cast<uptr>(p) + TNodeOffset);
    }

    void PushBack(T* p) { insert(end(), GetNodeFromPointer(p)); }
    void Erase(T* p) { erase(GetNodeFromPointer(p)); }
};
} // namespace ut
} // namespace nw
