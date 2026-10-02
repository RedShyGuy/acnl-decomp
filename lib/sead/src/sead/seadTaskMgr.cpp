#include "sead/seadTaskBase.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadTaskMgr.h"

namespace sead {
// ctor address unknown
sead::TaskMgr::TaskMgr()
{
}

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::TaskMgr::vf_0x00()
{
}

// 0x0055F31C | nintendogs:callseq-callee [tier A]
void sead::TaskMgr::createHeap_(sead::HeapArray*, const sead::TaskBase::CreateArg&)
{
}

// 0x0055F530 | nintendogs:callseq [tier A]
void sead::TaskMgr::appendToList_(sead::TList<sead::TaskBase*>&, sead::TaskBase*)
{
}

// 0x0055F640 | nintendogs:callseq [tier A]
void sead::TaskMgr::calcCreation_()
{
}

// 0x0055F830 | nintendogs:callseq [tier A]
void sead::TaskMgr::doCreateTask_(const sead::TaskBase::CreateArg&, sead::HeapArray*)
{
}

// 0x0055F900 | nintendogs:callseq [tier A]
void sead::TaskMgr::createTaskSync(const sead::TaskBase::CreateArg&)
{
}

// 0x0055FBCC | nintendogs:callseq [tier A]
void sead::TaskMgr::doDestroyTask_(sead::TaskBase*)
{
}

// 0x0055FC94 | nintendogs:callgraph [tier A]
void sead::TaskMgr::destroyTaskSync(sead::TaskBase*)
{
}

// 0x0055FE00 | nintendogs:callseq-callee [tier A]
void sead::TaskMgr::changeTaskState_(sead::TaskBase*, sead::TaskBase::State)
{
}

// 0x005601B8 | nintendogs:callgraph [tier A]
void sead::TaskMgr::destroyAllAndCreateRoot()
{
}

// 0x005605C0 | nintendogs:callseq [tier A]
void sead::TaskMgr::prepare_(sead::Thread*, int)
{
}

} // namespace sead
