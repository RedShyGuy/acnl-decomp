#pragma once

#include "decomp.h"
#include "nn/os/os_ThreadLocalStorage.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadTListNode.h"

namespace sead {
class Heap;
class Thread;

// RTTI N4sead9ThreadMgrE @ 0x008D23E4
// vtable 0x00907140 (vptr 0x00907148), offset_to_top 0, 3 entries
//
// The list of the threads, the main thread and the thread local storage that tells the current
// thread. A singleton created on a heap with its disposer in the same block (layout from the
// constructor 0x00139440; the member names are ours).
class ThreadMgr : public ::sead::hostio::Node
{
public:
    class SingletonDisposer_;

    ThreadMgr(); // 0x00139440 (name is ours)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual ~ThreadMgr(); // 0x00562B14 slot 0x04 | slot vf_0x04 of sead::ThreadMgr
    // 0x00562B04 slot 0x08 | slot vf_0x08 of sead::ThreadMgr (deleting dtor)

    // the instance on heap (with its disposer)
    static void createInstance(Heap* heap); // 0x00133888 (name is ours)
    // makes the main thread (name is ours)
    void initialize(Heap* heap); // 0x001393C0
    void initMainThread_(Heap* heap); // 0x001393C4 (name is ours)

    // the thread that runs (the value of the thread local storage)
    Thread* getCurrentThread() const { return reinterpret_cast<Thread*>(mThreadPtrTLS.GetValue()); }

    static ThreadMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4];          // 0x04, the SingletonDisposer_ (not set by the constructor)
    TList<Thread*> mList;                     // 0x14
    CriticalSection mListCS;                  // 0x20
    Thread* mMainThread;                      // 0x3C
    nn::os::ThreadLocalStorage mThreadPtrTLS; // 0x40
};
ASSERT_OFFSET(ThreadMgr, mList, 0x14);
ASSERT_SIZE(ThreadMgr, 0x44);
} // namespace sead
