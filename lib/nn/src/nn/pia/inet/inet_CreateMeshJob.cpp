#include "nn/pia/inet/inet_CreateMeshJob.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NexFacade.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E6A98 | slot vf_0x1C of nn::pia::session::CreateMeshJob
void nn::pia::inet::CreateMeshJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x003E6A9C | fefates:bytes
nn::Result nn::pia::inet::CreateMeshJob::StartupImpl()
{
    Reset(true);
    // (both branches of the original, with and without the NAT session, set this step)
    SetStep(&CreateMeshJob::SetupSystemProtocols, "CreateMeshJob::SetupSystemProtocols");
    common::g_SessionBeginMonitoringContent.m_Unknown0x58 = NexFacade::s_pInstance->m_Unknown0x10;
    return nn::Result();
}

// 0x003E6B44 | virtual slot, introduced by nn::pia::session::CreateMeshJob
void nn::pia::inet::CreateMeshJob::SetupMonitoringData()
{
    common::g_SessionBeginMonitoringContent.m_Unknown0x168 =
        common::hashWithMd5(NexFacade::s_pInstance->m_pMatchMakingClient->m_pDefaultCredentials->m_PrincipalId);
}

// 0x004310C8 | slot vf_0x00 of nn::pia::common::Job
// 0x003E6B74 (deleting dtor)
nn::pia::inet::CreateMeshJob::~CreateMeshJob()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
