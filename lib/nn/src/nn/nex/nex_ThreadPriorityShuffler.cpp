#include "nn/nex/nex_ThreadPriorityShuffler.h"
#include "nn/nex/nex_ObjectThreadRoot.h"
#include "nn/nex/nex_Platform.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
namespace {
// the time between the changes (ns)
const s64 SHUFFLE_INTERVAL = 1000000;
} // namespace

// 0x0037E4BC (name is ours)
void nn::nex::ThreadPriorityShuffler::Run(int unused)
{
    while (!m_IsStopped) {
        m_Lock.Enter();
        if (m_ThreadCount != 0) {
            s32 priority = Platform::GetRandomNumber(2) != 0 ? PRIORITY_LOW : PRIORITY_HIGH;
            ObjectThreadRoot* pThread = m_pThreads[Platform::GetRandomNumber(m_ThreadCount)];
            pThread->m_Priority = priority;
            nn::Result result = nn::svc::SetThreadPriority(pThread->m_pData->m_pThread->GetHandle(), nn::os::detail::ConvertLibraryToSvcPriority(priority));
            if (result.IsFailure()) {
                nn::os::CTR::detail::HandleInternalError(result);
            }
        }
        OsSleepThread(SHUFFLE_INTERVAL);
        m_Lock.Leave();
    }
}

} // namespace nex
} // namespace nn
