#include "sead/seadThreadMgr.h"
#include "nn/Handle.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"
#include "sead/seadHeap.h"
#include "sead/seadMainThread.h"
#include "sead/seadNew.h"
#include "sead/seadThreadMgr_SingletonDisposer_.h"

#include <new>

namespace sead {
// 0x009763F0
ThreadMgr* ThreadMgr::s_pInstance = nullptr;
// 0x009763F4
ThreadMgr::SingletonDisposer_* ThreadMgr::s_pSingletonDisposer = nullptr;

// 0x00139440 (name is ours)
sead::ThreadMgr::ThreadMgr() : mList(), mListCS(), mMainThread(nullptr), mThreadPtrTLS()
{
    mThreadPtrTLS.SetValue(0);
}

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::ThreadMgr::vf_0x00()
{
}

// 0x00562B14 slot 0x04 | slot vf_0x04 of sead::ThreadMgr
// 0x00562B04 (deleting dtor)
sead::ThreadMgr::~ThreadMgr()
{
    ScopedLock lock(&mListCS);
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
        nn::os::YieldThread();
        Thread::sleep(TickSpan::fromMilliSeconds(1));
    } while (!isDone);
    for (TListNode<Thread*>* node = mList.front(); node != mList.end(); node = TList<Thread*>::next(node)) {
        node->mData->waitDone();
    }
    s_pInstance = nullptr;
}

// 0x00133888 (name is ours)
void sead::ThreadMgr::createInstance(Heap* heap)
{
    if (s_pInstance != nullptr) {
        return;
    }
    u8* buffer = new (heap, 4) u8[sizeof(ThreadMgr)];
    s_pSingletonDisposer = new (buffer + offsetof(ThreadMgr, mSingletonDisposerBuffer)) SingletonDisposer_(heap);
    s_pInstance = new (buffer) ThreadMgr();
}

// 0x001393C0 (name is ours)
void sead::ThreadMgr::initialize(Heap* heap)
{
    initMainThread_(heap);
}

// 0x001393C4 (name is ours)
void sead::ThreadMgr::initMainThread_(Heap* heap)
{
    u32 id;
    nn::Result result = nn::svc::GetThreadId(&id, nn::Handle(nn::PSEUDO_HANDLE_CURRENT_THREAD));
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    MainThread* mainThread = new (heap, 4) MainThread(heap, &nn::os::Thread::s_MainThread, id);
    mMainThread = mainThread;
    mThreadPtrTLS.SetValue(reinterpret_cast<uptr>(mainThread));
}

} // namespace sead
