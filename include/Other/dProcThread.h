#pragma once

#include "decomp.h"
#include "Sv/dSvStepInner.h"
#include "sead/seadThread.h"

// the save thread ("SvProc Thread"): runs one SaveProcess for its request (made by SvStepInner::Start)
// RTTI 10ProcThread @ 0x008CB128
// vtable 0x008EC2C4 (vptr 0x008EC2CC), offset_to_top 0, 18 entries
// vtable 0x008EC314 (vptr 0x008EC31C), offset_to_top -24, 1 entries
class ProcThread : public ::sead::Thread
{
public:
    // (inline in SvStepInner::Start) heap: the current heap of the thread while it runs
    ProcThread(const sead::SafeStringBase<char>& name, sead::Heap* threadHeap, s32 priority, SvStepInner* request,
               sead::Heap* heap)
        : sead::Thread(name, threadHeap, priority, sead::MessageQueue::BLOCK_TYPE_NO_BLOCK, 0x7FFFFFFF, 0x5000, 32),
          m_pRequest(request), m_pHeap(heap)
    {
        m_pRequest->m_Result = SvStepInner::RESULT_RUNNING;
        m_pRequest->m_IsDone = false;
    }
    virtual ~ProcThread(); // 0x0055D99C slot 0x00
    // the save process for the request runs, then the thread quits
    void calc_(sead::MessageQueue::Element message) override; // 0x001AF76C slot 0x40 (name after sead::Thread)

    SvStepInner* m_pRequest;    // 0x90
    sead::Heap* m_pHeap;        // 0x94
};
ASSERT_SIZE(ProcThread, 0x98);
