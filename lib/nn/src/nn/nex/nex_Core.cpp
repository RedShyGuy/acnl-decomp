#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Scheduler_SchedulerWorkerThread.h"
#include "nn/nex/nex_SecurityContextManager.h"
#include "nn/nex/nex_SystemComponents.h"

namespace nn {
namespace nex {
// 0x0096C91D (name is ours)
bool Core::s_IsSchedulerThreaded;
// 0x0096C91E (name is ours)
u8 Core::s_ThreadMode;
// 0x0096C91F (name is ours)
bool Core::s_IsTransportThreaded;
// 0x0096C921 (name is ours)
bool Core::s_IsThreadSafe;
// 0x0096C980 (name is ours)
u32 Core::s_CoreCount;

// 0x003CD83C | fefates:callgraph [tier C]
nn::nex::Core* nn::nex::Core::GetInstance()
{
    InstanceControl* pInstance = InstanceTable::s_Instance.GetInstance(KIND, PseudoSingleton::GetCurrentContext());
    if (pInstance == nullptr) {
        return nullptr;
    }
    return static_cast<Core*>(pInstance->m_pOwner);
}

// 0x003CD994 | fefates:callseq [tier B]
void nn::nex::Core::AcquireInstance()
{
    InstanceTable::s_Lock.m_Mutex.Enter();
    GlobalVariables::AcquireInstance();
    if (GetInstance() != nullptr) {
        GetInstance()->AcquireRef();
    } else {
        new Core();
    }
    InstanceTable::s_Lock.m_Mutex.Leave();
}

// 0x003CDA04 | fefates:callgraph [tier C]
void nn::nex::Core::ReleaseInstance()
{
    InstanceTable::s_Lock.m_Mutex.Enter();
    GetInstance()->ReleaseRef();
    if (s_CoreCount == 0) {
        CriticalSection::s_IsLockDisabled = false;
    }
    GlobalVariables::ReleaseInstance();
    InstanceTable::s_Lock.m_Mutex.Leave();
}

// 0x003CDA84 (name is ours)
void nn::nex::Core::ApplyThreadMode()
{
    switch (s_ThreadMode) {
    case 0:
        break;
    case 1:
        s_IsSchedulerThreaded = false;
        s_IsTransportThreaded = true;
        s_IsThreadSafe = true;
        break;
    case 2:
        s_IsSchedulerThreaded = false;
        s_IsTransportThreaded = true;
        s_IsThreadSafe = false;
        break;
    case 3:
        s_IsSchedulerThreaded = false;
        s_IsTransportThreaded = false;
        s_IsThreadSafe = false;
        break;
    case 4:
        s_IsSchedulerThreaded = false;
        s_IsTransportThreaded = false;
        s_IsThreadSafe = true;
        break;
    case 5:
        s_IsSchedulerThreaded = true;
        s_IsTransportThreaded = false;
        s_IsThreadSafe = true;
        break;
    case 6:
        s_IsSchedulerThreaded = true;
        s_IsTransportThreaded = true;
        s_IsThreadSafe = true;
        break;
    }
}

// 0x003CDB08 (name is ours)
void nn::nex::Core::SetInterrupted(bool isInterrupted)
{
    void (*pCallback)(void*) = nullptr;
    void* pContext = nullptr;
    {
        ScopedCS lock(m_Lock);
        if (m_IsInterrupted == isInterrupted) {
            return;
        }
        m_IsInterrupted = isInterrupted;
        if (isInterrupted) {
            m_InterruptTime = SystemClock::GetTime();
            if (m_pInterruptCallback != nullptr) {
                pCallback = m_pInterruptCallback;
                pContext = m_pInterruptContext;
                m_pInterruptCallback = nullptr;
            }
        }
    }
    if (pCallback != nullptr) {
        pCallback(pContext);
    }
}

// 0x003CDBC8 | fefates:callseq [tier B]
nn::nex::Core::Core()
    : m_Singleton(KIND), m_IsInterrupted(false), m_pInterruptCallback(nullptr), m_pInterruptContext(nullptr), m_Unknown0x48(0), m_Lock(0x20000000)
{
    CallContextRegister::s_IsCancelDisabled = false;
    SystemClock::GetTime();
    ApplyThreadMode();
    if (s_IsSchedulerThreaded && !s_IsThreadSafe) {
        s_IsThreadSafe = true;
        CriticalSection::s_IsLockDisabled = false;
    } else if (!s_IsThreadSafe) {
        CriticalSection::s_IsLockDisabled = true;
    } else {
        CriticalSection::s_IsLockDisabled = false;
    }
    {
        ScopedCS lock(InstanceTable::s_Lock);
        s_CoreCount++;
    }
    {
        ScopedCS lock(InstanceTable::s_Lock);
        m_Singleton.m_pOwner = this;
    }
    m_pScheduler = new Scheduler(8, nullptr);
    if (s_IsSchedulerThreaded) {
        m_pScheduler->m_pWorkerThread->Launch(true);
    }
    m_pCallContextRegister = new CallContextRegister();
    m_pSystemComponents = new SystemComponents();
    m_pSystemComponents->m_pCoreGroup->RegisterComponent(m_pCallContextRegister);
    m_pSecurityContextManager = new SecurityContextManager();
    {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        m_pSystemComponents->m_pCoreGroup->Initialize();
    }
}

// 0x003CDEB4
// 0x003CDE84 (deleting dtor)
nn::nex::Core::~Core()
{
    {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        m_pSystemComponents->m_pCoreGroup->Terminate();
    }
    m_pSystemComponents->WaitForTerminatedState(static_cast<unsigned int>(-1));
    m_pScheduler->WaitForAllJobsToComplete(60000, true);
    m_pSystemComponents->m_pCoreGroup->UnregisterComponent(m_pCallContextRegister);
    delete m_pScheduler;
    delete m_pSystemComponents;
    delete m_pSecurityContextManager;
    {
        ScopedCS lock(InstanceTable::s_Lock);
        s_CoreCount--;
    }
    {
        ScopedCS lock(GlobalVariables::s_Lock);
        if (GlobalVariables::s_pInstance != nullptr) {
            GlobalVariables::s_pInstance->m_RootCaCache.clear();
        }
    }
}

} // namespace nex
} // namespace nn
