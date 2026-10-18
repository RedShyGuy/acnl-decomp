#include "nn/nex/nex_NATTraversalReportInternalProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"

namespace nn {
namespace nex {
// 0x00AE6BBC (name is ours)
NATTraversalReportInternalProtocolClient::ProtoReturnFunction NATTraversalReportInternalProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;

// 0x003CD070 slot 0x50
void nn::nex::NATTraversalReportInternalProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    u32 methodId = ExtractMethodID(pMessage) & 0x7FFF;
    if (methodId == 0 || methodId > METHOD_COUNT) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    ProtoReturnFunction pFunction = s_ProtoReturnFunctions[methodId - 1];
    if (pFunction != nullptr) {
        pFunction(pMessage, pContext);
    }
}

// 0x003CD0F4
nn::nex::NATTraversalReportInternalProtocolClient::NATTraversalReportInternalProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        NATTraversalReportInternalProtocolClient* pResponder = new NATTraversalReportInternalProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003CD1EC slot 0x00
// 0x003CD1DC (deleting dtor)
nn::nex::NATTraversalReportInternalProtocolClient::~NATTraversalReportInternalProtocolClient()
{
    // nothing to do
}

// 0x0072E084 slot 0x54
nn::nex::ClientProtocol* nn::nex::NATTraversalReportInternalProtocolClient::CreateResponder() const
{
    return new NATTraversalReportInternalProtocolClient(0);
}

} // namespace nex
} // namespace nn
