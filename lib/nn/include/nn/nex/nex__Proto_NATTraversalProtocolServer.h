#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_ServerProtocol.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class DateTime;
class String;

// The server side of the NAT traversal protocol: the requests are passed to the slots of the
// derived class. The method names of slots 0x5C-0x68 are from the reference symbols of
// NATTraversalRelayProtocol, the others are ours.
// RTTI N2nn3nex33_Proto_NATTraversalProtocolServerE @ 0x008CF334
class _Proto_NATTraversalProtocolServer : public ::nn::nex::ServerProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_REQUEST_PROBE_INITIATION = 1,
        METHOD_INITIATE_PROBE = 2,
        METHOD_REQUEST_PROBE_INITIATION_EXT = 3,
        METHOD_REPORT_NAT_TRAVERSAL_RESULT = 4,
        METHOD_REPORT_NAT_PROPERTIES = 5,
        METHOD_GET_RELAY_PARAMETERS = 6,
        METHOD_REPORT_NAT_TRAVERSAL_RESULT_DETAIL = 7
    };

    // (inline)
    _Proto_NATTraversalProtocolServer() : ServerProtocol(FLAG_REGISTER) {}
    // (symbols.json also names it the dispatch of NATTraversalRelayProtocol)
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint); // 0x003CB194 slot 0x50
    virtual void RequestProbeInitiation(const nn::nex::qList<nn::nex::StationURL>& targets) = 0; // slot 0x5C
    virtual void InitiateProbe(const nn::nex::StationURL& url) = 0; // slot 0x60
    virtual void RequestProbeInitiationExt(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source) = 0; // slot 0x64
    virtual void ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt) = 0; // slot 0x68
    virtual void ReportNATProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt) = 0; // slot 0x6C
    // the results: the parameters of the relay (the time as a DateTime)
    virtual void GetRelayParameters(unsigned int* pMode, nn::nex::DateTime* pTime, nn::nex::String* pAddress, unsigned short* pPort, unsigned int* pAddressType,
                                    unsigned int* pServerTime) = 0; // slot 0x70
    virtual void ReportNATTraversalResultDetail(const unsigned int& cid, const bool& isSucceeded, const nn::nex::NATTraversalResult& detail, const unsigned int& rtt) = 0; // slot 0x74
};
ASSERT_SIZE(_Proto_NATTraversalProtocolServer, 0x40);

// (out of line in the original, with checks of the message)
template <>
void _Type_qlist<_Type_stationurl>::Extract(Message* pMessage, qList<StationURL>* pList);
} // namespace nex
} // namespace nn
