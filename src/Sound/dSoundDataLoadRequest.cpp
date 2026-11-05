#include "Sound/dSoundDataLoadRequest.h"
#include "Sound/dSoundDataLoadThread.h"

// 0x00128EEC
SoundDataLoadRequest::SoundDataLoadRequest() : m_State(STATE_NONE), m_pHeap(NULL), m_ItemId(0xFFFFFFFF)
{
}

// 0x0013E4E0
// 0x003220C4 (deleting dtor)
SoundDataLoadRequest::~SoundDataLoadRequest()
{
    Cancel();
}

// 0x0032206C
void SoundDataLoadRequest::Request(nw::snd::SoundHeap* heap, u32 itemId)
{
    if (IsBusy()) {
        return;
    }
    m_pHeap = heap;
    m_State = STATE_QUEUED;
    m_ItemId = itemId;
    SoundDataLoadThread::AddRequest(this);
}

// 0x0013E474
void SoundDataLoadRequest::Cancel()
{
    sead::ScopedLock lock(&SoundDataLoadThread::s_CriticalSection);
    if (IsBusy()) {
        SoundDataLoadThread::RemoveRequest(this);
    }
    m_State = STATE_NONE;
    m_pHeap = NULL;
    m_ItemId = 0xFFFFFFFF;
}
