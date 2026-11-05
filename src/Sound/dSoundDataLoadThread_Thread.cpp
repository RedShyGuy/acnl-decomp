#include "Sound/dSoundDataLoadThread_Thread.h"
#include "Sound/dSoundDataLoadRequest.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/snd_SoundHeap.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"

// 0x002FF098
// 0x002FF088 (deleting dtor)
SoundDataLoadThread::Thread::~Thread()
{
    // empty (in the original too)
}

// 0x002FEE84
void SoundDataLoadThread::Thread::calc_(sead::MessageQueue::Element)
{
    for (;;) {
        sead::ScopedLock lock(&s_CriticalSection);
        SoundDataLoadRequest* request = reinterpret_cast<SoundDataLoadRequest*>(s_RequestList.GetFirst());
        if (request == NULL) {
            s_IsBusy = false;
            return;
        }
        request->m_State = SoundDataLoadRequest::STATE_LOADING;
        if (SoundMgr::s_pInstance->m_pPlayer->m_pSoundDataMgr->loadData(request->m_ItemId, 0xFFFFFFFF, 0x20000,
                                                                         request->m_pHeap)) {
            request->m_State = SoundDataLoadRequest::STATE_DONE;
            request->m_pHeap->SaveState();
        } else {
            request->m_State = SoundDataLoadRequest::STATE_FAILED;
            // what was loaded goes away again
            request->m_pHeap->LoadState(request->m_pHeap->GetCurrentLevel());
        }
        RemoveRequest(request);
    }
}
