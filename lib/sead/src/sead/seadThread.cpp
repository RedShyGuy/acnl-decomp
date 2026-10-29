#include "sead/seadThread.h"
#include "nn/Handle.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/os/os_Tick.h"
#include "nn/svc/svc_Api.h"
#include "sead/seadNew.h"
#include "sead/seadThreadMgr.h"

namespace sead {
namespace {
// 0x008AF128
const nn::Handle CURRENT_THREAD(nn::PSEUDO_HANDLE_CURRENT_THREAD);
} // namespace

// 0x00125D9C (name is ours)
sead::Thread::Thread(const SafeStringBase<char>& name, Heap* heap, s32 priority, MessageQueue::BlockType blockType,
                     MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize)
    : IDisposer(), INamable(name), mMessageQueue(), mStackSize(stackSize), mListNode(), mCurrentHeap(nullptr),
      mFindContainHeapCache(0), mBlockType(blockType), mQuitMessage(quitMessage), mId(0), mState(STATE_INITIALIZED),
      mThreadInner(nullptr), mStack(nullptr), mPriority(priority), mCoreNo(nn::os::Thread::CORE_NO_DEFAULT)
{
    mListNode.mData = this;
    mMessageQueue.allocate(messageQueueSize, heap);
    mStack = new (heap, 4) u8[stackSize];
    mThreadInner = new (heap, 4) nn::os::Thread();
    ThreadMgr* mgr = ThreadMgr::s_pInstance;
    if (mgr != nullptr) {
        ScopedLock lock(&mgr->mListCS);
        mgr->mList.pushBack(&mListNode);
    }
}

// 0x0013C734 (name is ours)
sead::Thread::Thread(Heap* heap, nn::os::Thread* threadInner, u32 id)
    : IDisposer(), INamable(SafeStringBase<char>("sead::MainThread")), mMessageQueue(), mStackSize(0), mListNode(),
      mCurrentHeap(nullptr), mFindContainHeapCache(0), mBlockType(MessageQueue::BLOCK_TYPE_NO_BLOCK),
      mQuitMessage(0x7FFFFFFF), mId(id), mState(STATE_RUNNING), mThreadInner(threadInner), mStack(nullptr)
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

// 0x0055D9A0 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0055D98C (deleting dtor)
sead::Thread::~Thread()
{
    ThreadMgr* mgr = ThreadMgr::s_pInstance;
    if (mgr == nullptr) {
        return;
    }
    if (mgr->mMainThread != this) {
        {
            ScopedLock lock(&mgr->mListCS);
            if (mListNode.mList != nullptr) {
                mgr->mList.erase(&mListNode);
            }
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
        delete mThreadInner;
        delete[] mStack;
    }
    mMessageQueue.free();
}

// 0x0055D938 slot 0x08
void sead::Thread::destroy()
{
    waitDone();
}

// 0x00540F8C slot 0x0C | nintendogs:bytes (name after pead::Thread)
bool sead::Thread::sendMessage(MessageQueue::Element message, MessageQueue::BlockType blockType)
{
    if (message == 0 || mState == STATE_TERMINATED || mState == STATE_RELEASED || message == mQuitMessage) {
        return false;
    }
    return mMessageQueue.push(message, blockType);
}

// 0x0055D67C slot 0x10
sead::MessageQueue::Element sead::Thread::recvMessage(MessageQueue::BlockType blockType)
{
    if (mState == STATE_QUITTING) {
        return 0;
    }
    return mMessageQueue.pop(blockType);
}

// 0x0055D83C slot 0x18 | nintendogs:bytes (name after pead::Thread)
bool sead::Thread::start()
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

// 0x0055D7C0 slot 0x1C (name after pead::Thread)
void sead::Thread::quit(bool isJam)
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

// 0x0055D944 slot 0x20 (name after pead::Thread)
void sead::Thread::waitDone()
{
    if (mState == STATE_INITIALIZED || mState == STATE_RELEASED) {
        return;
    }
    mThreadInner->Join();
    mState = STATE_RELEASED;
}

// 0x0055D788 slot 0x24
void sead::Thread::quitAndDestroySingleThread(bool isJam)
{
    quitAndWaitDoneSingleThread(isJam);
}

// 0x0055D794 slot 0x28 | nintendogs:bytes (name after pead::Thread)
void sead::Thread::quitAndWaitDoneSingleThread(bool isJam)
{
    quit(isJam);
    waitDone();
}

// 0x0055D698 slot 0x2C | nintendogs:bytes (name after pead::Thread)
void sead::Thread::setPriority(s32 priority)
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

// 0x0055D800 | nintendogs:bytes (name after pead::Thread)
void sead::Thread::run_()
{
    for (;;) {
        MessageQueue::Element message = mMessageQueue.pop(static_cast<MessageQueue::BlockType>(mBlockType));
        if (message == mQuitMessage) {
            return;
        }
        calc_(message);
    }
}

// 0x0055D728 | nintendogs:bytes (name after pead::Thread)
void sead::Thread::ctrThreadFunc_(uptr arg)
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

// 0x00139190 (name is ours)
void sead::Thread::sleep(TickSpan span)
{
    nn::os::Thread::SleepImpl(nn::os::Tick(span.mSpan).ToTimeSpan());
}

} // namespace sead

// the TypeInfo functions of the uptr threads are in os_Thread.cpp
