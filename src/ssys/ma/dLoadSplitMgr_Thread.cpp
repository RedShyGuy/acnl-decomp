#include "ssys/ma/dLoadSplitMgr_Thread.h"
#include "ssys/ma/dLoadSplit.h"

namespace ssys {
namespace ma {
// 0x00567BB4 slot 0x40
void ssys::ma::LoadSplitMgr::Thread::calc_(sead::MessageQueue::Element)
{
    while (s_SuspendCount <= 0) {
        s_QueueCriticalSection.lock();
        LoadSplit* loadSplit = static_cast<LoadSplit*>(s_Queue.GetFirst());
        if (loadSplit == NULL) {
            s_QueueCriticalSection.unlock();
            break;
        }
        if (loadSplit->m_State == LoadSplit::STATE_REQUESTED) {
            loadSplit->m_State = LoadSplit::STATE_LOADING;
        }
        s_QueueCriticalSection.unlock();
        if (loadSplit->m_State == LoadSplit::STATE_LOADING && !loadSplit->IsFinished()) {
            do {
                loadSplit->Update();
                s_LoadCount++;
                if (loadSplit->m_IsCancelRequested) {
                    break;
                }
            } while (!loadSplit->IsFinished());
        }
        if (loadSplit->m_State == LoadSplit::STATE_CANCELING) {
            loadSplit->Reset();
        }
        s_QueueCriticalSection.lock();
        if (s_Queue.Contains(loadSplit)) {
            RemoveFromQueue(loadSplit);
            if (loadSplit->m_IsCancelRequested) {
                loadSplit->m_State = LoadSplit::STATE_CANCELED;
            }
        }
        s_QueueCriticalSection.unlock();
    }
    s_IsBusy = false;
}

// 0x00567D08 slot 0x00
// 0x00567CF8 (deleting dtor)
ssys::ma::LoadSplitMgr::Thread::~Thread()
{
    // empty (in the original too)
}

} // namespace ma
} // namespace ssys
