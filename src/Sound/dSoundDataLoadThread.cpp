#include "Sound/dSoundDataLoadThread.h"
#include "Sound/dSoundDataLoadThread_Thread.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "sead/seadTickSpan.h"

// 0x0094DD28
bool SoundDataLoadThread::s_IsBusy = false;
// 0x0094DD2C
SoundDataLoadThread::Thread* SoundDataLoadThread::s_pThread = NULL;
// 0x0094DD30
sead::ExpHeap* SoundDataLoadThread::s_pHeap = NULL;
// 0x009838A0
sead::CriticalSection SoundDataLoadThread::s_CriticalSection;
// 0x00983894
ssys::st::List SoundDataLoadThread::s_RequestList;
// builds the lock and the list
// 0x0079CB34 (static initializer)

// 0x0011F540
void SoundDataLoadThread::Create(sead::Heap* heap)
{
    if (s_pHeap != NULL) {
        return;
    }
    s_pHeap = sead::ExpHeap::create(0x1400, sead::SafeStringBase<char>("SoundDataLoadThread"), heap,
                                    sead::Heap::HEAP_DIRECTION_FORWARD, false);
    if (s_pHeap == NULL) {
        return;
    }
    s_RequestList.Clear();
    s_IsBusy = false;
    s_pThread = new (s_pHeap, 4) Thread(sead::SafeStringBase<char>("SoundDataLoadThread"), s_pHeap,
                                        sead::Thread::DEFAULT_PRIORITY + 1);
    s_pThread->start();
}

// 0x0013608C
void SoundDataLoadThread::Destroy()
{
    if (s_pThread != NULL) {
        if (s_pThread->mState != sead::Thread::STATE_TERMINATED && s_pThread->mState != sead::Thread::STATE_RELEASED) {
            s_pThread->quit(false);
            s_pThread->waitDone();
        }
        s_pThread->destroy();
        delete s_pThread;
        s_pThread = NULL;
    }
    if (s_pHeap != NULL) {
        s_pHeap->destroy();
        s_pHeap = NULL;
    }
}

// 0x002FF09C
void SoundDataLoadThread::Wake()
{
    if (s_pThread == NULL || s_RequestList.GetFirst() == NULL) {
        return;
    }
    sead::Thread::sleep(sead::TickSpan::fromMilliSeconds(1));
    if (s_IsBusy) {
        return;
    }
    s_IsBusy = true;
    s_pThread->sendMessage('load', sead::MessageQueue::BLOCK_TYPE_BLOCK);
}
