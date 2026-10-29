#include "sead/seadFramework.h"
#include "sead/seadExpHeap.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadGlobalRandom.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadTaskMgr.h"
#include "sead/seadThreadMgr.h"

namespace sead {
// 0x0012D0FC (name is ours)
void sead::Framework::initialize(const InitializeArg& arg)
{
    if (arg.arena != nullptr) {
        HeapMgr::initialize(arg.arena);
    } else {
        HeapMgr::initialize(arg.heapSize);
    }
    Heap* root = HeapMgr::s_RootHeaps.size() > 0 ? HeapMgr::s_RootHeaps.at(0) : nullptr;
    ExpHeap* heap = ExpHeap::create(0, "sead::ThreadMgr", root, Heap::HEAP_DIRECTION_FORWARD, false);
    ThreadMgr::createInstance(heap);
    ThreadMgr::s_pInstance->initialize(heap);
    heap->adjust();
    GlobalRandom::createInstance(root);
}

// (out of line: the derived constructors call it)
// 0x0013374C | mk7dlp:bytes
DECOMP_NOINLINE sead::Framework::Framework()
    : mResetRequested(false), mResetParameter(nullptr), mResetEvent(), mTaskMgr(nullptr), mMethodTreeMgr(nullptr),
      mMethodTreeMgrHeap(nullptr)
{
}

// 0x00562600 slot 0x34 | nintendogs:bytes
void sead::Framework::procReset_()
{
    if (!mResetRequested) {
        return;
    }
    mResetEvent.fire(mResetParameter);
    mTaskMgr->destroyAllAndCreateRoot();
    mResetRequested = false;
    mResetParameter = nullptr;
}

// 0x00562670 slot 0x24 (name is ours)
void sead::Framework::requestReset(void* parameter)
{
    mResetParameter = parameter;
    mResetRequested = true;
}

// 0x00562680 slot 0x14 (name is ours)
void sead::Framework::createSystemTasks(TaskBase*, const CreateSystemTaskArg&)
{
    // empty (in the original too)
}

// 0x00562684 slot 0x20
bool sead::Framework::vf_0x20()
{
    return false;
}

// 0x0056268C (name is ours)
sead::Framework::CreateSystemTaskArg::CreateSystemTaskArg()
    : field_0x00(0), field_0x04(0), span(0), stackSize(0x1000)
{
}

// 0x005626B0 slot 0x10 (name is ours)
void sead::Framework::run(Heap* heap, const TaskBase::CreateArg& rootTaskArg, const RunArg& runArg)
{
    initRun_(heap);
    Heap* methodTreeMgrHeap = ExpHeap::create(0, "sead::MethodTreeMgr", heap, Heap::HEAP_DIRECTION_FORWARD, false);
    mMethodTreeMgr = createMethodTreeMgr_(methodTreeMgrHeap);
    methodTreeMgrHeap->adjust();
    mMethodTreeMgrHeap = methodTreeMgrHeap;
    TaskMgr::InitializeArg arg;
    arg.createQueueSize = 32;
    arg.prepareStackSize = 0x8000;
    arg.preparePriority = -1;
    arg.rootTaskCreateArg = &rootTaskArg;
    arg.heap = heap;
    arg.parentFramework = this;
    if (runArg.prepareStackSize != 0) {
        arg.prepareStackSize = runArg.prepareStackSize;
    }
    if (runArg.preparePriority != -1) {
        arg.preparePriority = runArg.preparePriority;
    }
    mTaskMgr = TaskMgr::create(arg);
    runImpl_();
}

// 0x0056279C slot 0x28 (name is ours)
void sead::Framework::initRun_(Heap*)
{
    // empty (in the original too)
}

// 0x005627A0 slot 0x2C | slot vf_0x2C of sead::Framework
void sead::Framework::runImpl_()
{
    // empty (in the original too)
}

// 0x005627B4 slot 0x08
// 0x005627A4 (deleting dtor)
sead::Framework::~Framework()
{
    if (mTaskMgr != nullptr) {
        mTaskMgr->finalize();
        delete mTaskMgr;
        mTaskMgr = nullptr;
    }
    if (mMethodTreeMgr != nullptr) {
        delete mMethodTreeMgr;
        mMethodTreeMgr = nullptr;
    }
    if (mMethodTreeMgrHeap != nullptr) {
        mMethodTreeMgrHeap->destroy();
    }
}
} // namespace sead
