#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadFramework.h"
#include "sead/seadGameFramework.h"
#include "sead/seadGraphics.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadProcessMeter.h"
#include "sead/seadTaskBase_CreateArg.h"
#include "sead/seadTaskMgr.h"
#include "sead/seadThread.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x0012C7B0 (name is ours)
DECOMP_NOINLINE sead::GameFramework::GameFramework()
    : Framework(), mDisplayState(0), mString0(""), mString1(""), mString2(""), mThread(nullptr),
      mDrawLockFunction(&lockDrawContext_), field_0x4C(0)
{
}

// 0x00132BB8 (name is ours)
void sead::GameFramework::lockDrawContext_(bool lock)
{
    if (lock) {
        Graphics::s_pInstance->lockDrawContext();
    } else {
        Graphics::s_pInstance->unlockDrawContext();
    }
}

// 0x0054102C slot 0x40 (name is ours)
void sead::GameFramework::createProcessMeter(TaskBase* rootTask)
{
    ProcessMeter::createInstance(rootTask->mHeapArray.getPrimaryHeap());
}

// 0x00542EC8 slot 0x50
void sead::GameFramework::vf_0x50()
{
    // empty (in the original too)
}

// 0x00542ECC slot 0x3C
void sead::GameFramework::vf_0x3C(TaskBase*, u32, u32)
{
    // empty (in the original too)
}

// 0x00542ED0 slot 0x44
void sead::GameFramework::vf_0x44(TaskBase*)
{
    // empty (in the original too)
}

// 0x00542ED4 slot 0x14 (name is ours)
void sead::GameFramework::createSystemTasks(TaskBase* rootTask, const CreateSystemTaskArg& arg)
{
    // (the base gets a new argument, not arg)
    Framework::createSystemTasks(rootTask, CreateSystemTaskArg());
    createControllerMgr(rootTask);
    createProcessMeter(rootTask);
    vf_0x44(rootTask);
    vf_0x3C(rootTask, arg.field_0x00, arg.field_0x04);
    vf_0x48(rootTask, arg.span, arg.stackSize);
}

// 0x00542F78 slot 0x38 | nintendogs:callseq
void sead::GameFramework::createControllerMgr(TaskBase* rootTask)
{
    SystemTaskCreateArg arg(TaskClassID(&TaskFactory<ControllerMgr>));
    arg.parent = rootTask;
    mTaskMgr->createSingletonTaskSync<ControllerMgr>(arg);
}

// 0x00543040 slot 0x48
void sead::GameFramework::vf_0x48(TaskBase*, const TickSpan&, s32)
{
    // empty (in the original too)
}

// 0x00543048 slot 0x58 | nintendogs:bytes-fuzzy
void sead::GameFramework::waitStartDisplayLoop_()
{
    for (;;) {
        Graphics::s_pInstance->lockDrawContext();
        mTaskMgr->calcCreation_();
        mTaskMgr->calcDestruction_();
        Graphics::s_pInstance->unlockDrawContext();
        if (mTaskMgr->mRootTask != nullptr || mDisplayState != 0) {
            break;
        }
        Thread::sleep(TickSpan::fromMilliSeconds(10));
    }
    mMethodTreeMgr->pauseAll(false);
    if (mDisplayState == 0) {
        mDisplayState = 1;
    }
}

// 0x00543170 slot 0x08
// 0x00543110 (deleting dtor)
sead::GameFramework::~GameFramework()
{
    if (mThread != nullptr) {
        mThread->quitAndDestroySingleThread(false);
        delete mThread;
    }
}
} // namespace sead

// the type information of the classes below Framework
// 0x008298EC (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Framework>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
