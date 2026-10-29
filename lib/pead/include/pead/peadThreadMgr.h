#pragma once

#include "decomp.h"
#include "nn/os/os_ThreadLocalStorage.h"
#include "pead/hostio/peadNode.h"
#include "pead/peadTListNode.h"

namespace pead {
class Heap;
class Thread;

// RTTI N4pead9ThreadMgrE @ 0x008D1340
// vtable 0x00904DE4 (vptr 0x00904DEC), offset_to_top 0, 2 entries
//
// pia's copy of sead::ThreadMgr: the list of the threads (without a lock), the main thread and the thread local
// storage that tells the current thread. A singleton created on a heap with its disposer in the same block
// (layout from the constructor 0x0053DE74; the member names are ours).
class ThreadMgr : public ::pead::hostio::Node
{
public:
    class SingletonDisposer_;

    ThreadMgr(); // 0x0053DE74 | nintendogs:bytes [tier A]
    virtual ~ThreadMgr(); // 0x0053DEBC slot 0x00 | nintendogs:bytes
    // 0x0053DEAC slot 0x04 | slot vf_0x04 of pead::ThreadMgr (deleting dtor)

    static ThreadMgr* instance() { return s_pInstance; }
    // the instance on heap with its disposer (pia's common::HeapManager; the names are ours)
    static ThreadMgr* createInstance(Heap* heap); // 0x0053DC2C
    static void deleteInstance(); // 0x0053DCC0
    // makes the MainThread object of the calling thread on the heap (a nop that falls into initMainThread_)
    void initialize(Heap* heap); // 0x0053DD18
    void initMainThread_(Heap* heap); // 0x0053DD1C (name after sead)
    // deletes the MainThread object (name is ours)
    void destroy(); // 0x0053DE3C

    // the thread that runs (the value of the thread local storage)
    Thread* getCurrentThread() const { return reinterpret_cast<Thread*>(mThreadPtrTLS.GetValue()); }

    static ThreadMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4];          // 0x04, the SingletonDisposer_ (not set by the constructor)
    TList<Thread*> mList;                     // 0x14
    Thread* mMainThread;                      // 0x20
    nn::os::ThreadLocalStorage mThreadPtrTLS; // 0x24
};
ASSERT_OFFSET(ThreadMgr, mList, 0x14);
ASSERT_SIZE(ThreadMgr, 0x28);
} // namespace pead
