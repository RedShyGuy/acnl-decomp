#pragma once

#include "decomp.h"
#include "pead/hostio/peadReflexible.h"
#include "pead/peadIDisposer.h"
#include "pead/peadINamable.h"
#include "pead/peadMessageQueue.h"
#include "pead/peadTListNode.h"

namespace nn {
namespace os {
class Thread;
} // namespace os
} // namespace nn

namespace pead {
class Heap;

// RTTI N4pead6ThreadE @ 0x008D128C
// vtable 0x00904CE8 (vptr 0x00904CF0), offset_to_top 0, 17 entries
//
// pia's copy of sead::Thread (an older one): a thread of the system that runs calc_ for each message of its
// queue until the quit message comes. Unlike sead there is no getMessageQueue slot (the slots from 0x14 on are
// 4 bytes lower), no cache of findContainHeap, no lock of the thread list, and calcStackUsedSizePeak measures
// the stack. The names sendMessage/start/quitAndWaitDoneSingleThread/setPriority/run_/ctrThreadFunc_ are from
// the reference symbols; the other names are those of our sead.
class Thread : public ::pead::IDisposer, public ::pead::INamable, public ::pead::hostio::Reflexible
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

    // the pattern of an unused stack word (calcStackUsedSizePeak)
    static const u32 STACK_FILL_PATTERN = 0x5EAD5CEC;

    Thread(const SafeStringBase<char>& name, Heap* heap, s32 priority, MessageQueue::BlockType blockType,
           MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize); // 0x0053C3EC (name after sead)
    // the thread that runs main (its system thread exists already; name "NAMESPACE_NAME::MainThread")
    Thread(Heap* heap, nn::os::Thread* threadInner, u32 id); // 0x0053C328 (name after sead)
    virtual ~Thread(); // 0x0053C574 slot 0x00 | nintendogs:callgraph
    // 0x0053C560 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void destroy(); // 0x0053C2D0 slot 0x08 (name after sead)
    virtual bool sendMessage(MessageQueue::Element message, MessageQueue::BlockType blockType); // 0x005383C4 slot 0x0C | nintendogs:bytes
    virtual MessageQueue::Element recvMessage(MessageQueue::BlockType blockType); // 0x0053BFA0 slot 0x10 (name after sead)
    virtual bool start(); // 0x0053C1C0 slot 0x14 | nintendogs:bytes
    virtual void quit(bool isJam); // 0x0053C0F0 slot 0x18 (name after sead)
    virtual void waitDone(); // 0x0053C2DC slot 0x1C (name after sead)
    virtual void quitAndDestroySingleThread(bool isJam); // 0x0053C0B8 slot 0x20 (name after sead)
    virtual void quitAndWaitDoneSingleThread(bool isJam); // 0x0053C0C4 slot 0x24 | nintendogs:bytes
    virtual void setPriority(s32 priority); // 0x0053BFBC slot 0x28 | nintendogs:bytes
    virtual s32 getPriority() const // 0x00749534 slot 0x2C (name after sead)
    {
        return mPriority;
    }
    virtual MessageQueue::BlockType getBlockType() const // 0x0074953C slot 0x30 (name after sead)
    {
        return static_cast<MessageQueue::BlockType>(mBlockType);
    }
    virtual s32 getStackSize() const // 0x00749544 slot 0x34 (name after sead)
    {
        return mStackSize;
    }
    // the bytes of the stack that were used (the first word from the low end that lost the fill pattern)
    virtual s32 calcStackUsedSizePeak() const // 0x0074954C slot 0x38 (name after sead)
    {
        const u32* word = static_cast<const u32*>(getStack());
        const u32* end = reinterpret_cast<const u32*>(mStack + mStackSize);
        s32 num = end - word + 1;
        for (s32 i = 1; i < num; i++, word++) {
            if (*word != STACK_FILL_PATTERN) {
                return reinterpret_cast<const u8*>(end) - reinterpret_cast<const u8*>(word);
            }
        }
        return 0;
    }
    // the work for one message
    virtual void calc_(MessageQueue::Element message) = 0;
    virtual void* getStack() const // 0x007495BC slot 0x40 (name after sead)
    {
        return mStack;
    }

    // waits for the message, runs calc_ until the quit message
    void run_(); // 0x0053C138 | nintendogs:bytes [tier A]
    // the function of the system thread
    static void ctrThreadFunc_(uptr arg); // 0x0053C04C | nintendogs:bytes [tier A]
    // gives the processor to another thread (svcSleepThread(0); name is ours)
    static void yield(); // 0x0053C2C4

    MessageQueue mMessageQueue;            // 0x18
    s32 mStackSize;                        // 0x50
    TListNode<Thread*> mListNode;          // 0x54, in the list of ThreadMgr
    Heap* mCurrentHeap;                    // 0x64
    u8 mBlockType;                         // 0x68
    MessageQueue::Element mQuitMessage;    // 0x6C
    u32 mId;                               // 0x70, of the thread of the system
    volatile s32 mState;                   // 0x74
    nn::os::Thread* mThreadInner;          // 0x78
    u8* mStack;                            // 0x7C
    s32 mPriority;                         // 0x80
    s32 mCoreNo;                           // 0x84
};
ASSERT_OFFSET(Thread, mMessageQueue, 0x18);
ASSERT_OFFSET(Thread, mCurrentHeap, 0x64);
ASSERT_SIZE(Thread, 0x88);
} // namespace pead
