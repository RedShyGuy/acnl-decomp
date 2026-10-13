#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
class CallContextRegister;
class Scheduler;
class SecurityContextManager;
class SystemComponents;

// The core of nex: the scheduler, the call contexts and the system components of a context. The
// names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex4CoreE @ 0x008CF514
// vtable 0x008FF990 (vptr 0x008FF998), offset_to_top 0, 2 entries
class Core : public ::nn::nex::RefCountedObject
{
public:
    static const u32 KIND = 3;

    Core(); // 0x003CDBC8 | fefates:callseq [tier B]
    virtual ~Core(); // 0x003CDEB4 slot 0x00
    // 0x003CDE84 slot 0x04 (deleting dtor)

    // the core of the current context (null if there is none)
    static Core* GetInstance(); // 0x003CD83C | fefates:callgraph [tier C]
    // makes the core (and the GlobalVariables) or takes a reference to it
    static void AcquireInstance(); // 0x003CD994 | fefates:callseq [tier B]
    static void ReleaseInstance(); // 0x003CDA04 | fefates:callgraph [tier C]
    // sets the threading flags from s_ThreadMode
    static void ApplyThreadMode(); // 0x003CDA84 (name is ours)
    // while interrupted, the timeouts of the core count as expired; the callback runs (once) when
    // it is interrupted
    void SetInterrupted(bool isInterrupted); // 0x003CDB08 (name is ours)

    Scheduler* m_pScheduler;                          // 0x0C
    CallContextRegister* m_pCallContextRegister;      // 0x10
    SystemComponents* m_pSystemComponents;            // 0x14
    SecurityContextManager* m_pSecurityContextManager; // 0x18
    PseudoSingleton m_Singleton;                      // 0x1C (its owner is the core)
    bool m_IsInterrupted;                             // 0x30
    Time m_InterruptTime;                             // 0x38
    void (*m_pInterruptCallback)(void* pContext);     // 0x40
    void* m_pInterruptContext;                        // 0x44
    u8 m_Unknown0x48;                                 // 0x48
    CriticalSection m_Lock;                           // 0x4C

    static u8 s_ThreadMode;
    // the scheduler has a worker thread
    static bool s_IsSchedulerThreaded;
    static bool s_IsTransportThreaded;
    // locks are used
    static bool s_IsThreadSafe;
    static u32 s_CoreCount;
};
ASSERT_SIZE(Core, 0x78);
} // namespace nex
} // namespace nn
