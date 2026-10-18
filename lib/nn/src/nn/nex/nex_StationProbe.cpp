#include "nn/nex/nex_StationProbe.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x00360F58 | fefates:bytes [tier B]
void nn::nex::StationProbe::Reset()
{
    m_URL.Copy(StationURL(L""));
    m_RoundTripTime = static_cast<u32>(-1);
}

// 0x00360F9C (name is ours)
void nn::nex::StationProbe::Update()
{
    NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
    bool isFinished = true;
    for (qList<StationURL>::Node* pNode = m_pURLs->m_pNext; pNode != m_pURLs; pNode = pNode->m_pNext) {
        const StationURL& url = pNode->m_Value;
        u32 roundTripTime;
        {
            ScopedCS lock(GetSchedulerLock());
            URLProbe* pProbe = pEngine->m_FinishedProbes.FindProbe(url);
            roundTripTime = pProbe != nullptr ? pProbe->m_Result : static_cast<u32>(-1);
        }
        if (roundTripTime != static_cast<u32>(-1) && roundTripTime < m_RoundTripTime) {
            m_RoundTripTime = roundTripTime;
            // (the URL as updated by the probe)
            ScopedCS lock(GetSchedulerLock());
            URLProbe* pProbe = pEngine->m_FinishedProbes.FindProbe(url);
            if (pProbe != nullptr && pProbe->m_URL != url) {
                m_URL.Copy(pProbe->m_URL);
            } else {
                m_URL.Copy(url);
            }
        }
        if (!pEngine->IsProbeFailed(url)) {
            isFinished = false;
        }
    }
    if (isFinished) {
        m_IsFinished = true;
    }
}

} // namespace nex
} // namespace nn
