#include "nn/Handle.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/os/os_Tick.h"
#include "nn/svc/svc_Api.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadThread.h"
#include "pead/peadThreadMgr.h"
#include "pead/peadTickSpan.h"

#include <new>

namespace pead {
namespace {
// 0x008B16B8
const nn::Handle CURRENT_THREAD(nn::PSEUDO_HANDLE_CURRENT_THREAD);
} // namespace

// 0x005383C4 slot 0x0C | nintendogs:bytes
bool pead::Thread::sendMessage(MessageQueue::Element message, MessageQueue::BlockType blockType)
{
    if (message == 0 || mState == STATE_TERMINATED || mState == STATE_RELEASED || message == mQuitMessage) {
        return false;
    }
    return mMessageQueue.push(message, blockType);
}

// 0x0053BFA0 slot 0x10 (name after sead)
pead::MessageQueue::Element pead::Thread::recvMessage(MessageQueue::BlockType blockType)
{
    if (mState == STATE_QUITTING) {
        return 0;
    }
    return mMessageQueue.pop(blockType);
}

// 0x0053BFBC slot 0x28 | nintendogs:bytes
void pead::Thread::setPriority(s32 priority)
{
    mPriority = priority;
    if (mState == STATE_INITIALIZED || mState == STATE_RELEASED) {
        return;
    }
    nn::Result result;
    if (ThreadMgr::s_pInstance->getCurrentThread() == this) {
        result = nn::svc::SetThreadPriority(CURRENT_THREAD, nn::os::detail::ConvertLibraryToSvcPriority(priority));
    } else {
        result = nn::svc::SetThreadPriority(mThreadInner->GetHandle(),
                                            nn::os::detail::ConvertLibraryToSvcPriority(priority));
    }
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
}

// 0x0053C04C | nintendogs:bytes [tier A]
void pead::Thread::ctrThreadFunc_(uptr arg)
{
    Thread* thread = reinterpret_cast<Thread*>(arg);
    ThreadMgr::s_pInstance->mThreadPtrTLS.SetValue(arg);
    u32 id;
    nn::Result result = nn::svc::GetThreadId(&id, CURRENT_THREAD);
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    thread->mId = id;
    thread->mState = STATE_RUNNING;
    thread->run_();
    thread->mState = STATE_TERMINATED;
}

// 0x0053C0B8 slot 0x20 (name after sead)
void pead::Thread::quitAndDestroySingleThread(bool isJam)
{
    quitAndWaitDoneSingleThread(isJam);
}

// 0x0053C0C4 slot 0x24 | nintendogs:bytes
void pead::Thread::quitAndWaitDoneSingleThread(bool isJam)
{
    quit(isJam);
    waitDone();
}

// 0x0053C0F0 slot 0x18 (name after sead)
void pead::Thread::quit(bool isJam)
{
    if (mState == STATE_TERMINATED || mState == STATE_RELEASED) {
        return;
    }
    if (mState == STATE_RUNNING) {
        mState = STATE_QUITTING;
    }
    if (isJam) {
        mMessageQueue.jam(mQuitMessage, MessageQueue::BLOCK_TYPE_BLOCK);
    } else {
        mMessageQueue.push(mQuitMessage, MessageQueue::BLOCK_TYPE_BLOCK);
    }
}

// 0x0053C138 | nintendogs:bytes [tier A]
void pead::Thread::run_()
{
    for (;;) {
        MessageQueue::Element message = mMessageQueue.pop(static_cast<MessageQueue::BlockType>(mBlockType));
        if (message == mQuitMessage) {
            return;
        }
        calc_(message);
    }
}

// 0x0053C174 (name is ours)
void SleepThread(TickSpan span)
{
    nn::os::Thread::SleepImpl(nn::os::Tick(span.mSpan).ToTimeSpan());
}

// 0x0053C1C0 slot 0x14 | nintendogs:bytes
bool pead::Thread::start()
{
    if (mState != STATE_INITIALIZED) {
        return false;
    }
    nn::Result result = mThreadInner->TryStart(&ctrThreadFunc_, reinterpret_cast<uptr>(this),
                                               reinterpret_cast<uptr>(mStack + mStackSize), mPriority, mCoreNo);
    if (result.IsFailure()) {
        return false;
    }
    u32 id;
    result = nn::svc::GetThreadId(&id, mThreadInner->GetHandle());
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    mId = id;
    if (mState == STATE_INITIALIZED) {
        mState = STATE_RUNNING;
    }
    return true;
}

// 0x0053C2C4 (name is ours)
void pead::Thread::yield()
{
    svcSleepThread(0);
}

// 0x0053C2D0 slot 0x08 (name after sead)
void pead::Thread::destroy()
{
    waitDone();
}

// 0x0053C2DC slot 0x1C (name after sead)
void pead::Thread::waitDone()
{
    if (mState == STATE_INITIALIZED || mState == STATE_RELEASED) {
        return;
    }
    mThreadInner->Join();
    mState = STATE_RELEASED;
}

// 0x0053C328 (name after sead)
pead::Thread::Thread(Heap* heap, nn::os::Thread* threadInner, u32 id)
    : IDisposer(), INamable(SafeStringBase<char>("NAMESPACE_NAME::MainThread")), mMessageQueue(), mStackSize(0),
      mListNode(), mCurrentHeap(nullptr), mBlockType(MessageQueue::BLOCK_TYPE_NO_BLOCK), mQuitMessage(0x7FFFFFFF),
      mId(id), mState(STATE_RUNNING), mThreadInner(threadInner), mStack(nullptr)
{
    s32 priority;
    nn::Result result = nn::svc::GetThreadPriority(&priority, threadInner->GetHandle());
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    mPriority = nn::os::detail::ConvertSvcToLibraryPriority(priority);
    mCoreNo = nn::os::Thread::CORE_NO_DEFAULT;
    mListNode.mData = this;
    mMessageQueue.allocate(32, heap);
}

// 0x0053C3EC (name after sead)
pead::Thread::Thread(const SafeStringBase<char>& name, Heap* heap, s32 priority, MessageQueue::BlockType blockType,
                     MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize)
    : IDisposer(), INamable(name), mMessageQueue(), mStackSize(stackSize), mListNode(), mCurrentHeap(nullptr),
      mBlockType(blockType), mQuitMessage(quitMessage), mId(0), mState(STATE_INITIALIZED), mThreadInner(nullptr),
      mStack(nullptr), mPriority(priority), mCoreNo(nn::os::Thread::CORE_NO_DEFAULT)
{
    mListNode.mData = this;
    mMessageQueue.allocate(messageQueueSize, heap);
    mStack = NewArray<u8>(stackSize, heap);
    mThreadInner = ::new (AllocMemory(sizeof(nn::os::Thread), heap)) nn::os::Thread();
    ThreadMgr* mgr = ThreadMgr::s_pInstance;
    if (mgr != nullptr) {
        mgr->mList.pushBack(&mListNode);
    }
}

// 0x0053C574 slot 0x00 | nintendogs:callgraph
// 0x0053C560 (deleting dtor)
pead::Thread::~Thread()
{
    ThreadMgr* mgr = ThreadMgr::s_pInstance;
    if (mgr == nullptr) {
        return;
    }
    if (mgr->mMainThread != this) {
        if (mListNode.mList != nullptr) {
            mgr->mList.erase(&mListNode);
        }
        switch (mState) {
        case STATE_RUNNING:
            quitAndWaitDoneSingleThread(false);
            break;
        case STATE_QUITTING:
        case STATE_TERMINATED:
            waitDone();
            break;
        default:
            break;
        }
        mThreadInner->Finalize();
        if (mThreadInner != nullptr) {
            mThreadInner->~Thread();
            FreeMemory(mThreadInner);
        }
        if (mStack != nullptr) {
            DeleteArray(mStack);
        }
    }
    mMessageQueue.free();
}
} // namespace pead
