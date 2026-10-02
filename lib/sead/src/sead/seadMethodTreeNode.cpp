#include "sead/seadTTreeNode.h"
#include "sead/seadINamable.h"
#include "sead/seadIDisposer.h"
#include "sead/seadMethodTreeNode.h"

namespace sead {
// TODO: default ctor added so derived stubs compile - may not exist
sead::MethodTreeNode::MethodTreeNode()
{
}

// 0x00545410 slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::MethodTreeNode::~MethodTreeNode()
{
}

// 0x0074C368 slot 0x08 | virtual slot, introduced by sead::MethodTreeNode
void sead::MethodTreeNode::vf_0x08()
{
}

// 0x0074C31C slot 0x0C | virtual slot, introduced by sead::MethodTreeNode
void sead::MethodTreeNode::vf_0x0C()
{
}

// 0x0012C964 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::lock_()
{
}

// 0x0012C9E0 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::unlock_()
{
}

// 0x0012C9F4 | nintendogs:bytes [tier A]
sead::MethodTreeNode::MethodTreeNode(sead::CriticalSection*)
{
}

// 0x00545154 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::pushBackChild(sead::MethodTreeNode*)
{
}

// 0x005451F4 | nintendogs:callseq [tier A]
void sead::MethodTreeNode::pushFrontChild(sead::MethodTreeNode*)
{
}

// 0x0054529C | nintendogs:bytes [tier A]
void sead::MethodTreeNode::call()
{
}

// 0x00545314 | nintendogs:bytes [tier A]
void sead::MethodTreeNode::callRec_()
{
}

// 0x00545374 | nintendogs:bytes [tier A]
void sead::MethodTreeNode::detachAll()
{
}

// 0x0074C2D8 | nintendogs:bytes [tier A]
void sead::MethodTreeNode::attachMutexRec_(sead::CriticalSection*) const
{
}

} // namespace sead
