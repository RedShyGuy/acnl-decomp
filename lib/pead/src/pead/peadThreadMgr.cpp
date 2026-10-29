#include "nn/Handle.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadMainThread.h"
#include "pead/peadThreadMgr.h"
#include "pead/peadThreadMgr_SingletonDisposer_.h"

#include <cstddef>
#include <new>

namespace pead {
// 0x0097E43C
ThreadMgr* ThreadMgr::s_pInstance = nullptr;
// 0x0097E440
ThreadMgr::SingletonDisposer_* ThreadMgr::s_pSingletonDisposer = nullptr;

// 0x0053DC2C (name is ours)
pead::ThreadMgr* pead::ThreadMgr::createInstance(Heap* heap)
{
    if (s_pInstance != nullptr) {
        return s_pInstance;
    }
    u8* buffer = NewArray<u8>(sizeof(ThreadMgr), heap);
    s_pSingletonDisposer = ::new (buffer + offsetof(ThreadMgr, mSingletonDisposerBuffer)) SingletonDisposer_();
    ThreadMgr* mgr = nullptr;
    if (buffer != nullptr) {
        mgr = ::new (buffer) ThreadMgr();
    }
    s_pInstance = mgr;
    return mgr;
}

// 0x0053DCC0 (name is ours)
void pead::ThreadMgr::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == nullptr) {
        return;
    }
    s_pSingletonDisposer = nullptr;
    disposer->~SingletonDisposer_();
    ThreadMgr* mgr = s_pInstance;
    if (mgr != nullptr) {
        mgr->~ThreadMgr();
        FreeMemory(mgr);
    }
    s_pInstance = nullptr;
}

// 0x0053DD18 (name is ours)
void pead::ThreadMgr::initialize(Heap* heap)
{
    initMainThread_(heap);
}

// 0x0053DD1C (name after sead)
void pead::ThreadMgr::initMainThread_(Heap* heap)
{
    u32 id;
    nn::Result result = nn::svc::GetThreadId(&id, nn::Handle(nn::PSEUDO_HANDLE_CURRENT_THREAD));
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    MainThread* mainThread = ::new (AllocMemory(sizeof(MainThread), heap)) MainThread(heap, &nn::os::Thread::s_MainThread, id);
    mMainThread = mainThread;
    mThreadPtrTLS.SetValue(reinterpret_cast<uptr>(mainThread));
}

// 0x0053DE3C (name is ours)
void pead::ThreadMgr::destroy()
{
    Thread* mainThread = mMainThread;
    if (mainThread != nullptr) {
        mainThread->~Thread();
        FreeMemory(mainThread);
        mMainThread = nullptr;
    }
}

// 0x0053DE74 | nintendogs:bytes [tier A]
pead::ThreadMgr::ThreadMgr() : mList(), mMainThread(nullptr), mThreadPtrTLS()
{
    // (only the members are initialized)
}

// 0x0053DEBC slot 0x00 | nintendogs:bytes
// 0x0053DEAC (deleting dtor)
pead::ThreadMgr::~ThreadMgr()
{
    for (TListNode<Thread*>* node = mList.front(); node != mList.end(); node = TList<Thread*>::next(node)) {
        node->mData->quit(false);
    }
    bool isDone;
    do {
        isDone = true;
        for (TListNode<Thread*>* node = mList.front(); node != mList.end(); node = TList<Thread*>::next(node)) {
            s32 state = node->mData->mState;
            isDone &= state == Thread::STATE_TERMINATED || state == Thread::STATE_RELEASED;
        }
        Thread::yield();
    } while (!isDone);
    for (TListNode<Thread*>* node = mList.front(); node != mList.end(); node = TList<Thread*>::next(node)) {
        node->mData->waitDone();
    }
}
} // namespace pead
