#include "Other/dProcThread.h"

#include "Other/dSaveProcess.h"
#include "applet/dAppletState.h"
#include "sead/seadHeapMgr.h"

// 0x0055D99C
// 0x001AF800 (deleting dtor)
ProcThread::~ProcThread()
{
    // empty (in the original too)
}

// 0x001AF76C (name after sead::Thread)
void ProcThread::calc_(sead::MessageQueue::Element message)
{
    (void)message;
    applet::IncrementSaveThreadCount();
    {
        sead::ScopedCurrentHeapSetter setter(m_pHeap);
        SaveProcess process(m_pRequest);
        process.Run();
    }
    quit(false);
    applet::DecrementSaveThreadCount();
}
