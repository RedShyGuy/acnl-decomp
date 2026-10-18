#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class ProtocolCallContext;
class StationURL;

// The client of the NAT traversal protocol (protocol id 3), used by the NAT traversal relay
// protocol. The method names are from the reference symbols of the relay client, the names of the
// members are ours.
// RTTI N2nn3nex26NATTraversalProtocolClientE @ 0x008CEF88
// vtable 0x008FEDD4 (vptr 0x008FEDDC), offset_to_top 0, 23 entries
class NATTraversalProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_REQUEST_PROBE_INITIATION_EXT = 3,
        METHOD_REPORT_NAT_TRAVERSAL_RESULT = 4,
        METHOD_REPORT_NAT_PROPERTIES = 5,
        METHOD_COUNT = 7
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE NATTraversalProtocolClient(unsigned short id); // 0x003BA68C
    virtual ~NATTraversalProtocolClient(); // 0x003BA784 slot 0x00
    // 0x003BA774 slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BA308 slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DDB8 slot 0x54 | fefates:callseq

    // (the return types are ours)
    bool CallReportNATProperties(nn::nex::ProtocolCallContext* pContext, const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x003BA1A0
    bool CallReportNATTraversalResult(nn::nex::ProtocolCallContext* pContext, const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt); // 0x003BA38C
    bool CallRequestProbeInitiationExt(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source); // 0x003BA4FC
    // (no results)
    static void ProtoReturn_ReportNATProperties(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BA680
    static void ProtoReturn_ReportNATTraversalResult(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BA684
    static void ProtoReturn_RequestProbeInitiationExt(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BA688

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(NATTraversalProtocolClient, 0x44);
} // namespace nex
} // namespace nn
