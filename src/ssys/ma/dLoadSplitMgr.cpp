#include "ssys/ma/dLoadSplitMgr.h"
#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/dLoadSplitMgr_Thread.h"
#include "nn/os/detail/detail_Api.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

namespace ssys {
namespace ma {
// 0x00976C50
bool LoadSplitMgr::s_IsBusy = false;
// 0x00976C51
s8 LoadSplitMgr::s_SuspendCount = 0;
// 0x00976C52
s8 LoadSplitMgr::s_OpenFileCount = 0;
// 0x00976C54
u32 LoadSplitMgr::s_DefaultAlignment = 16;
// 0x00976C58
u32 LoadSplitMgr::s_DefaultChunkSize = 0x80000;
// 0x00976C5C
LoadSplitMgr::Thread* LoadSplitMgr::s_pThread = NULL;
// 0x00976C60
sead::ExpHeap* LoadSplitMgr::s_pHeap = NULL;
// 0x00976C64
u32 LoadSplitMgr::s_LoadCount = 0;
// 0x00976C70
s32 LoadSplitMgr::s_ThreadPriority = 0;
// 0x00AE67D4
::ssys::st::List LoadSplitMgr::s_Queue;
// 0x00AE67E0
sead::CriticalSection LoadSplitMgr::s_QueueCriticalSection;
// 0x00AE67FC
sead::CriticalSection LoadSplitMgr::s_OpenFileCriticalSection;

// 0x00567AA4 (name is ours)
void ssys::ma::LoadSplitMgr::Initialize(sead::Heap* heap)
{
    LoadSplit::SetDefaultChunkSize(0x20000);
    CreateThread(heap);
}

// 0x00567AC0 (name is ours)
void ssys::ma::LoadSplitMgr::CreateThread(sead::Heap* heap)
{
    s_pHeap = sead::ExpHeap::create(0x1400, sead::SafeStringBase<char>("LoadSplitMgr"), heap,
                                    sead::Heap::HEAP_DIRECTION_FORWARD, false);
    s_Queue.Clear();
    s_IsBusy = false;
    nn::os::detail::StartAlarmThreadPool(0);
    s_pThread = new (s_pHeap, 4) Thread(sead::SafeStringBase<char>("LoadSplitMgr"), s_pHeap, s_ThreadPriority,
                                        sead::MessageQueue::BLOCK_TYPE_BLOCK, 0x7FFFFFFF, 0x1000, 32);
    s_pThread->start();
    s_pHeap->adjust();
}

// 0x00567D0C (name is ours)
void ssys::ma::LoadSplitMgr::Wake()
{
    if (s_SuspendCount > 0) {
        return;
    }
    s_LoadCount = 0;
    if (s_pThread == NULL || s_Queue.GetFirst() == NULL || s_IsBusy) {
        return;
    }
    s_IsBusy = true;
    s_pThread->sendMessage('load', sead::MessageQueue::BLOCK_TYPE_BLOCK);
}

// 0x0012D20C (name is ours)
void ssys::ma::LoadSplitMgr::Finalize()
{
    if (s_pThread != NULL) {
        if (s_pThread->mState != sead::Thread::STATE_TERMINATED && s_pThread->mState != sead::Thread::STATE_RELEASED) {
            s_pThread->quit(false);
            s_pThread->waitDone();
        }
        s_pThread->destroy();
        if (s_pThread != NULL) {
            delete s_pThread;
        }
        s_pThread = NULL;
    }
    if (s_pHeap != NULL) {
        s_pHeap->destroy();
        s_pHeap = NULL;
    }
}

} // namespace ma
} // namespace ssys
