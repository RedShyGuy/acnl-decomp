#pragma once

#include "decomp.h"
#include "sead/seadCriticalSection.h"
#include "ssys/st/dList.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead
class SoundDataLoadRequest;

// The thread that loads the queued SoundDataLoadRequests (the class name and the name of the thread are from the
// binary, everything else is ours).
class SoundDataLoadThread
{
public:
    class Thread;

    // the thread and its heap "SoundDataLoadThread"
    static void Create(sead::Heap* heap); // 0x0011F540
    static void Destroy(); // 0x0013608C
    // wakes the thread up if there is a request
    static void Wake(); // 0x002FF09C

    static void AddRequest(SoundDataLoadRequest* request)
    {
        sead::ScopedLock lock(&s_CriticalSection);
        s_RequestList.PushBack(reinterpret_cast<ssys::st::ListNode*>(request));
    }
    static void RemoveRequest(SoundDataLoadRequest* request)
    {
        sead::ScopedLock lock(&s_CriticalSection);
        s_RequestList.Erase(reinterpret_cast<ssys::st::ListNode*>(request));
    }

    static bool s_IsBusy;          // the thread works through the list
    static Thread* s_pThread;
    static sead::ExpHeap* s_pHeap;
    static ssys::st::List s_RequestList;
    static sead::CriticalSection s_CriticalSection;
};
