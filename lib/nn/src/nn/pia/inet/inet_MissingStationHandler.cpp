#include "nn/pia/inet/inet_MissingStationHandler.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;
} // namespace

// 0x00401570 | fefates:bytes
void nn::pia::inet::MissingStationHandler::Execute(nn::pia::transport::StationConnectionInfo* pInfo)
{
    // the traversal to the station that left is no longer needed
    NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(pInfo->m_PublicLocation, true);
    pInfo->Trace(TRACE_FLAG);
}

// 0x004015B4
// 0x004015B0 (deleting dtor)
nn::pia::inet::MissingStationHandler::~MissingStationHandler()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
