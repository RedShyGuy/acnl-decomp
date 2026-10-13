#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_MethodCallJob.h"
#include "nn/nex/nex_PeriodicJob.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SystemSetting.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the period of CheckExpiredCalls (ms)
// 0x00AE04A4 (name is ours)
SystemSetting s_CheckExpiredCallsPeriod(L"CallContextRegisterCheckExpiredCalls", 100);
} // namespace

// 0x0096C91C (name is ours)
bool CallContextRegister::s_IsCancelDisabled;

// 0x00391A54 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::Unregister(unsigned int id)
{
    m_CallContexts.erase(m_CallContexts.find(id));
    std::pair<std::multimap<u32, u32>::iterator, std::multimap<u32, u32>::iterator> range = m_Associations.equal_range(id);
    m_Associations.erase(range.first, range.second);
}

// 0x00391B78 | fefates:bytes [tier B]
nn::nex::CallContext* nn::nex::CallContextRegister::GetCallContext(unsigned int id)
{
    qMap<u32, CallContext*>::iterator it = m_CallContexts.find(id);
    if (it == m_CallContexts.end()) {
        return nullptr;
    }
    return it->second;
}

// 0x00391BD8 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::GetAssociatedId(unsigned int id, nn::nex::qVector<unsigned int>* pIds)
{
    pIds->clear();
    std::pair<std::multimap<u32, u32>::iterator, std::multimap<u32, u32>::iterator> range = m_Associations.equal_range(id);
    for (std::multimap<u32, u32>::iterator it = range.first; it != range.second; ++it) {
        pIds->push_back(it->second);
    }
}

// 0x00391D20 | fefates:bytes
bool nn::nex::CallContextRegister::BeginTermination()
{
    Core* pCore = Core::GetInstance();
    Scheduler* pScheduler = pCore != nullptr ? pCore->m_pScheduler : nullptr;
    if (!pScheduler->Cancel(m_pTimeoutJob)) {
        m_pTimeoutJob->SetState(Job::STATE_COMPLETED);
    }
    m_pTimeoutJob->ReleaseRef();
    m_pTimeoutJob = nullptr;
    return true;
}

// 0x00391DA8 | fefates:bytes-fuzzy [tier B]
void nn::nex::CallContextRegister::CancelCallContexts(void* pOwner, unsigned int ownerId)
{
    qVector<u32> cancelledIds;
    qMap<u32, CallContext*>::iterator it = m_CallContexts.begin();
    while (it != m_CallContexts.end()) {
        CallContext* pCallContext = it->second;
        if (pCallContext->m_State == CallContext::STATE_CALL_IN_PROGRESS &&
            (pCallContext->m_pOwner == pOwner || (ownerId != 0 && pCallContext->m_OwnerId == ownerId))) {
            qVector<u32> ids;
            Core::GetInstance()->m_pCallContextRegister->GetAssociatedId(pCallContext->m_Id, &ids);
            cancelledIds.insert(cancelledIds.end(), ids.begin(), ids.end());
            qMap<u32, CallContext*>::iterator current = it;
            ++it;
            current->second->SetStateImpl(CallContext::STATE_FAILURE, qResult(static_cast<int>(0x80010004)), true);
        } else {
            ++it;
        }
    }
    for (u32* pId = cancelledIds.begin(); pId != cancelledIds.end(); pId++) {
        CallContext* pCallContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(*pId);
        if (pCallContext != nullptr) {
            pCallContext->Cancel(CallContext::STATE_CANCELLED);
        }
    }
}

// 0x00391FD4 (name from the debug string of its job)
void nn::nex::CallContextRegister::CheckExpiredCalls(int unused)
{
    Time now = SystemClock::GetTime();
    qVector<u32> cancelledIds;
    qMap<u32, CallContext*>::iterator it = m_CallContexts.begin();
    while (it != m_CallContexts.end()) {
        CallContext* pCallContext = it->second;
        if (pCallContext->m_State == CallContext::STATE_CALL_IN_PROGRESS && pCallContext->m_Deadline.m_Value != 0 &&
            pCallContext->m_Deadline.m_Value < now.m_Value) {
            qVector<u32> ids;
            Core::GetInstance()->m_pCallContextRegister->GetAssociatedId(pCallContext->m_Id, &ids);
            cancelledIds.insert(cancelledIds.end(), ids.begin(), ids.end());
            qMap<u32, CallContext*>::iterator current = it;
            ++it;
            current->second->SetStateImpl(CallContext::STATE_FAILURE, qResult(static_cast<int>(0x8001000B)), true);
        } else {
            ++it;
        }
    }
    for (u32* pId = cancelledIds.begin(); pId != cancelledIds.end(); pId++) {
        CallContext* pCallContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(*pId);
        if (pCallContext != nullptr) {
            pCallContext->Cancel(CallContext::STATE_CANCELLED);
        }
    }
}

// 0x003921F8 (slot name from the reference symbols)
bool nn::nex::CallContextRegister::BeginInitialization()
{
    Start();
    return true;
}

// 0x00392208 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::AssociateCallContext(unsigned int id, unsigned int associatedId)
{
    m_Associations.insert(std::pair<const u32, u32>(id, associatedId));
}

// 0x00392234
void nn::nex::CallContextRegister::vf_0x10()
{
    // nothing to do in this build
}

// 0x00392238 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::Start()
{
    m_pTimeoutJob = new MethodCallJob<CallContextRegister, int, PeriodicJob>(
        Job::JOB_TYPE_DEFAULT, DebugString(String(L"CallContextRegister::CheckExpiredCalls")), this, &CallContextRegister::CheckExpiredCalls, 0);
    m_pTimeoutJob->m_Period = s_CheckExpiredCallsPeriod.m_Value;
    m_pTimeoutJob->AcquireRef();
    Scheduler::GetInstance()->Queue(m_pTimeoutJob, false);
}

// 0x00392350 | fefates:bytes [tier B]
unsigned int nn::nex::CallContextRegister::Register(nn::nex::CallContext* pCallContext)
{
    u32 id = m_NextId;
    if (++m_NextId == 0) {
        m_NextId = 1;
    }
    m_CallContexts[id] = pCallContext;
    return id;
}

// 0x003924AC | fefates:bytes [tier B]
nn::nex::CallContextRegister::CallContextRegister() : SystemComponent(String(L"CallContextRegister")), m_NextId(1), m_pTimeoutJob(nullptr)
{
}

// 0x003926A0 | fefates:callseq
// 0x00392670 (deleting dtor)
nn::nex::CallContextRegister::~CallContextRegister()
{
    if (!s_IsCancelDisabled) {
        qMap<u32, CallContext*>::iterator it = m_CallContexts.begin();
        while (it != m_CallContexts.end()) {
            qMap<u32, CallContext*>::iterator current = it;
            ++it;
            current->second->SetStateImpl(CallContext::STATE_CANCELLED, qResult(), true);
        }
    }
}

// 0x0072CBA8
const wchar_t* nn::nex::CallContextRegister::GetType() const
{
    return L"CallContextRegister";
}

// 0x0072CBD8
bool nn::nex::CallContextRegister::IsAKindOf(const wchar_t* pType) const
{
    if (wcscmp(L"CallContextRegister", pType) == 0) {
        return true;
    }
    return wcscmp(L"SystemComponent", pType) == 0;
}

// 0x007D4690 | fefates:bytes [tier B]
template void nn::nex::MethodCallJob<nn::nex::CallContextRegister, int, nn::nex::PeriodicJob>::Execute();

// 0x007D46E8
// 0x007D46BC (deleting dtor)
template nn::nex::MethodCallJob<nn::nex::CallContextRegister, int, nn::nex::PeriodicJob>::~MethodCallJob();

// 0x00827A74
template nn::nex::String nn::nex::MethodCallJob<nn::nex::CallContextRegister, int, nn::nex::PeriodicJob>::GetTraceString() const;

} // namespace nex
} // namespace nn
