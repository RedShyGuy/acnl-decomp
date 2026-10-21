#include "nn/nex/nex_ProtocolCallObserver.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x009712E0 (name is ours)
ProtocolCallObserver* ProtocolCallObserver::s_pInstance;

// 0x0039A4FC (name is ours)
void nn::nex::ProtocolCallObserver::NotifyCallBegins()
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    if (s_pInstance != nullptr) {
        s_pInstance->OnCallBegins();
    }
}

// 0x0039A554 (name is ours)
void nn::nex::ProtocolCallObserver::NotifyCallEnds(nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    ProtocolCallObserver* pObserver = s_pInstance;
    if (pObserver != nullptr && pObserver->IsEnabled()) {
        pObserver->OnCallEnds(pEndPoint);
    }
}

} // namespace nex
} // namespace nn
