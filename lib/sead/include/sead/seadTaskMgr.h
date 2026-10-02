#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead7TaskMgrE @ 0x008D2150
// vtable 0x00906C94 (vptr 0x00906C9C), offset_to_top 0, 1 entries
class TaskMgr : public ::sead::hostio::Node
{
public:
    TaskMgr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    void createHeap_(sead::HeapArray*, const sead::TaskBase::CreateArg&); // 0x0055F31C | nintendogs:callseq-callee [tier A]
    void appendToList_(sead::TList<sead::TaskBase*>&, sead::TaskBase*); // 0x0055F530 | nintendogs:callseq [tier A]
    void calcCreation_(); // 0x0055F640 | nintendogs:callseq [tier A]
    void doCreateTask_(const sead::TaskBase::CreateArg&, sead::HeapArray*); // 0x0055F830 | nintendogs:callseq [tier A]
    void createTaskSync(const sead::TaskBase::CreateArg&); // 0x0055F900 | nintendogs:callseq [tier A]
    void doDestroyTask_(sead::TaskBase*); // 0x0055FBCC | nintendogs:callseq [tier A]
    void destroyTaskSync(sead::TaskBase*); // 0x0055FC94 | nintendogs:callgraph [tier A]
    void changeTaskState_(sead::TaskBase*, sead::TaskBase::State); // 0x0055FE00 | nintendogs:callseq-callee [tier A]
    void destroyAllAndCreateRoot(); // 0x005601B8 | nintendogs:callgraph [tier A]
    void prepare_(sead::Thread*, int); // 0x005605C0 | nintendogs:callseq [tier A]
};
} // namespace sead
