#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadTTreeNode.h"

namespace sead {
// RTTI N4sead14MethodTreeNodeE @ 0x008D171C
// vtable 0x009058D4 (vptr 0x009058DC), offset_to_top 0, 4 entries
class MethodTreeNode : public ::sead::TTreeNode<sead::MethodTreeNode*>, public ::sead::INamable, public ::sead::IDisposer
{
public:
    MethodTreeNode(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~MethodTreeNode(); // 0x00545410 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005453E4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074C368 slot 0x08 | virtual slot, introduced by sead::MethodTreeNode
    virtual void vf_0x0C(); // 0x0074C31C slot 0x0C | virtual slot, introduced by sead::MethodTreeNode
    void lock_(); // 0x0012C964 | nintendogs:callgraph [tier A]
    void unlock_(); // 0x0012C9E0 | nintendogs:callgraph [tier A]
    MethodTreeNode(sead::CriticalSection*); // 0x0012C9F4 | nintendogs:bytes [tier A]
    void pushBackChild(sead::MethodTreeNode*); // 0x00545154 | nintendogs:callgraph [tier A]
    void pushFrontChild(sead::MethodTreeNode*); // 0x005451F4 | nintendogs:callseq [tier A]
    void call(); // 0x0054529C | nintendogs:bytes [tier A]
    void callRec_(); // 0x00545314 | nintendogs:bytes [tier A]
    void detachAll(); // 0x00545374 | nintendogs:bytes [tier A]
    void attachMutexRec_(sead::CriticalSection*) const; // 0x0074C2D8 | nintendogs:bytes [tier A]
};
} // namespace sead
