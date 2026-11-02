#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"
#include "sead/seadCriticalSection.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
class LoadSplit;

// the queue of the async LoadSplits and the thread that loads them (only static members; all names except the class
// and Thread are ours)
class LoadSplitMgr
{
public:
    class Thread;

    // sets the default piece size to 128 KB and starts the thread
    static void Initialize(sead::Heap* heap); // 0x00567AA4
    static void CreateThread(sead::Heap* heap); // 0x00567AC0
    static void Finalize(); // 0x0012D20C
    // wakes the thread if there is work and it is not suspended or busy
    // (LoadSplit::Load has it inline)
    static void Wake(); // 0x00567D0C
    // (inline)
    static void RemoveFromQueue(LoadSplit* loadSplit);

    static bool s_IsBusy;                 // the thread works on the queue
    static s8 s_SuspendCount;
    static s8 s_OpenFileCount;            // files the LoadSplits have open
    static u32 s_DefaultAlignment;
    static u32 s_DefaultChunkSize;
    static Thread* s_pThread;
    static sead::ExpHeap* s_pHeap;
    static u32 s_LoadCount;               // pieces read since the last wake
    static s32 s_ThreadPriority;
    static ::ssys::st::List s_Queue;
    static sead::CriticalSection s_QueueCriticalSection;
    static sead::CriticalSection s_OpenFileCriticalSection;
};
} // namespace ma
} // namespace ssys
