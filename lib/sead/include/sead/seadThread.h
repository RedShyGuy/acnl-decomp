#pragma once

#include "decomp.h"
#include "sead/hostio/seadReflexible.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadMessageQueue.h"
#include "sead/seadTListNode.h"
#include "sead/seadTickSpan.h"

namespace nn {
namespace os {
class Thread;
} // namespace os
} // namespace nn

namespace sead {
class Heap;

// RTTI N4sead6ThreadE @ 0x008D20B0
// vtable 0x00906B7C (vptr 0x00906B84), offset_to_top 0, 18 entries
// vtable 0x00906BCC (vptr 0x00906BD4), offset_to_top -24, 1 entries
//
// A thread of the system that runs calc_ for each message of its queue until the quit message
// comes. Layout from the constructors 0x00125D9C and 0x0013C734. The names sendMessage, start,
// quitAndWaitDoneSingleThread, setPriority, run_ and ctrThreadFunc_ are from the reference symbols
// (of pead, whose vtable lacks slot 0x14: its slots from 0x14 on are 4 bytes lower); the other names
// are ours.
class Thread : public ::sead::IDisposer, public ::sead::INamable, public ::sead::hostio::Reflexible
{
public:
    // mState (names are ours)
    enum State
    {
        STATE_INITIALIZED = 0,
        STATE_RUNNING = 1,
        STATE_QUITTING = 2,
        STATE_TERMINATED = 3,
        STATE_RELEASED = 4, // waited for
    };

    Thread(const SafeStringBase<char>& name, Heap* heap, s32 priority, MessageQueue::BlockType blockType,
           MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize); // 0x00125D9C (name is ours)
    // the thread that runs main (its system thread exists already; name "sead::MainThread")
    Thread(Heap* heap, nn::os::Thread* threadInner, u32 id); // 0x0013C734 (name is ours)
    virtual ~Thread(); // 0x0055D9A0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0055D98C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void destroy(); // 0x0055D938 slot 0x08
    virtual bool sendMessage(MessageQueue::Element message, MessageQueue::BlockType blockType); // 0x00540F8C slot 0x0C
    virtual MessageQueue::Element recvMessage(MessageQueue::BlockType blockType); // 0x0055D67C slot 0x10
    virtual const MessageQueue& getMessageQueue() const // 0x0074EE58 slot 0x14
    {
        return mMessageQueue;
    }
    virtual bool start(); // 0x0055D83C slot 0x18
    virtual void quit(bool isJam); // 0x0055D7C0 slot 0x1C
    virtual void waitDone(); // 0x0055D944 slot 0x20
    virtual void quitAndDestroySingleThread(bool isJam); // 0x0055D788 slot 0x24
    virtual void quitAndWaitDoneSingleThread(bool isJam); // 0x0055D794 slot 0x28
    virtual void setPriority(s32 priority); // 0x0055D698 slot 0x2C
    virtual s32 getPriority() const // 0x0074EE40 slot 0x30
    {
        return mPriority;
    }
    virtual MessageQueue::BlockType getBlockType() const // 0x0074EE48 slot 0x34
    {
        return static_cast<MessageQueue::BlockType>(mBlockType);
    }
    virtual s32 getStackSize() const // 0x0074EE50 slot 0x38
    {
        return mStackSize;
    }
    // (0 in the release build)
    virtual s32 calcStackUsedSizePeak() const // 0x0074EE60 slot 0x3C
    {
        return 0;
    }
    // the work for one message
    virtual void calc_(MessageQueue::Element message) = 0;
    virtual void* getStack() const // 0x0074EE68 slot 0x44
    {
        return mStack;
    }

    // waits for the message, runs calc_ until the quit message
    void run_(); // 0x0055D800 | nintendogs:bytes (name after pead::Thread)
    // the function of the system thread
    static void ctrThreadFunc_(uptr arg); // 0x0055D728 | nintendogs:bytes (name after pead::Thread)
    // sleeps the calling thread
    static void sleep(TickSpan span); // 0x00139190

    MessageQueue mMessageQueue;     // 0x1C
    s32 mStackSize;                 // 0x54
    TListNode<Thread*> mListNode;   // 0x58, in the list of ThreadMgr
    Heap* mCurrentHeap;             // 0x68
    // the last heap found by HeapMgr::findContainHeap for the thread; bit 0 is set while it is used
    u32 mFindContainHeapCache;      // 0x6C
    u8 mBlockType;                  // 0x70
    MessageQueue::Element mQuitMessage; // 0x74
    u32 mId;                        // 0x78, of the thread of the system
    s32 mState;                     // 0x7C
    nn::os::Thread* mThreadInner;   // 0x80
    u8* mStack;                     // 0x84
    s32 mPriority;                  // 0x88
    s32 mCoreNo;                    // 0x8C
};
ASSERT_OFFSET(Thread, mMessageQueue, 0x1C);
ASSERT_OFFSET(Thread, mCurrentHeap, 0x68);
ASSERT_SIZE(Thread, 0x90);
} // namespace sead
