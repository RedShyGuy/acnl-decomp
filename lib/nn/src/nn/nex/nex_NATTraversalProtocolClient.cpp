#include "nn/nex/nex_NATTraversalProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
// 0x00AE6B9C (name is ours)
NATTraversalProtocolClient::ProtoReturnFunction NATTraversalProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;

// 0x003BA1A0 (name is ours)
bool nn::nex::NATTraversalProtocolClient::CallReportNATProperties(nn::nex::ProtocolCallContext* pContext, const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_REPORT_NAT_PROPERTIES)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&mapping), sizeof(mapping));
    message.AppendRaw(reinterpret_cast<const u8*>(&filtering), sizeof(filtering));
    message.AppendRaw(reinterpret_cast<const u8*>(&rtt), sizeof(rtt));
    s_ProtoReturnFunctions[METHOD_REPORT_NAT_PROPERTIES - 1] = ProtoReturn_ReportNATProperties;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003BA38C (name is ours)
bool nn::nex::NATTraversalProtocolClient::CallReportNATTraversalResult(nn::nex::ProtocolCallContext* pContext, const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_REPORT_NAT_TRAVERSAL_RESULT)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&cid), sizeof(cid));
    message << isSucceeded;
    message.AppendRaw(reinterpret_cast<const u8*>(&rtt), sizeof(rtt));
    s_ProtoReturnFunctions[METHOD_REPORT_NAT_TRAVERSAL_RESULT - 1] = ProtoReturn_ReportNATTraversalResult;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003BA4FC (name is ours)
bool nn::nex::NATTraversalProtocolClient::CallRequestProbeInitiationExt(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_REQUEST_PROBE_INITIATION_EXT)) {
        return false;
    }
    _Type_qlist<_Type_stationurl>::Add(&message, targets);
    _Type_stationurl::Add(&message, source);
    s_ProtoReturnFunctions[METHOD_REQUEST_PROBE_INITIATION_EXT - 1] = ProtoReturn_RequestProbeInitiationExt;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003BA680 (name is ours)
void nn::nex::NATTraversalProtocolClient::ProtoReturn_ReportNATProperties(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003BA684 (name is ours)
void nn::nex::NATTraversalProtocolClient::ProtoReturn_ReportNATTraversalResult(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003BA688 (name is ours)
void nn::nex::NATTraversalProtocolClient::ProtoReturn_RequestProbeInitiationExt(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003BA308 slot 0x50
void nn::nex::NATTraversalProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x003BA68C
nn::nex::NATTraversalProtocolClient::NATTraversalProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        NATTraversalProtocolClient* pResponder = new NATTraversalProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003BA784 slot 0x00
// 0x003BA774 (deleting dtor)
nn::nex::NATTraversalProtocolClient::~NATTraversalProtocolClient()
{
    // nothing to do
}

// 0x0072DDB8 slot 0x54
nn::nex::ClientProtocol* nn::nex::NATTraversalProtocolClient::CreateResponder() const
{
    return new NATTraversalProtocolClient(0);
}

} // namespace nex
} // namespace nn
