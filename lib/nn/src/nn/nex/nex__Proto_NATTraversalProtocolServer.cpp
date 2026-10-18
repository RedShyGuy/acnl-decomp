#include "nn/nex/nex__Proto_NATTraversalProtocolServer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
// the method id of a response
const u32 METHOD_RESPONSE_FLAG = 0x8000;
} // namespace

// 0x007D4248
template <>
void _Type_qlist<_Type_stationurl>::Extract(Message* pMessage, qList<StationURL>* pList)
{
    pList->ClearInline();
    u32 count;
    if (!pMessage->ExtractPrimitive(&count)) {
        return;
    }
    for (u32 i = 0; i < count && !pMessage->m_IsError; i++) {
        StationURL url;
        _Type_stationurl::Extract(pMessage, &url);
        if (pMessage->m_IsError) {
            pList->ClearInline();
            return;
        }
        pList->PushBackInline(url);
    }
}

// 0x003CB194 slot 0x50
void nn::nex::_Proto_NATTraversalProtocolServer::DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint)
{
    switch (static_cast<u16>(Protocol::ExtractMethodID(pRequest))) {
    case METHOD_REQUEST_PROBE_INITIATION: {
        qList<StationURL> targets;
        _Type_qlist<_Type_stationurl>::Extract(pRequest, &targets);
        if (!pRequest->m_IsError) {
            RequestProbeInitiation(targets);
            Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_REQUEST_PROBE_INITIATION);
        }
        break;
    }
    case METHOD_INITIATE_PROBE: {
        StationURL url;
        _Type_stationurl::Extract(pRequest, &url);
        if (!pRequest->m_IsError) {
            InitiateProbe(url);
            Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_INITIATE_PROBE);
        }
        break;
    }
    case METHOD_REQUEST_PROBE_INITIATION_EXT: {
        qList<StationURL> targets;
        _Type_qlist<_Type_stationurl>::Extract(pRequest, &targets);
        if (!pRequest->m_IsError) {
            StationURL source;
            _Type_stationurl::Extract(pRequest, &source);
            if (!pRequest->m_IsError) {
                RequestProbeInitiationExt(targets, source);
                Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_REQUEST_PROBE_INITIATION_EXT);
            }
        }
        break;
    }
    case METHOD_REPORT_NAT_TRAVERSAL_RESULT: {
        u32 cid;
        bool isSucceeded;
        u32 rtt;
        pRequest->ExtractPrimitive(&cid);
        if (pRequest->m_IsError) {
            break;
        }
        *pRequest >> isSucceeded;
        if (pRequest->m_IsError) {
            break;
        }
        pRequest->ExtractPrimitive(&rtt);
        if (pRequest->m_IsError) {
            break;
        }
        ReportNATTraversalResult(cid, isSucceeded, rtt);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_REPORT_NAT_TRAVERSAL_RESULT);
        break;
    }
    case METHOD_REPORT_NAT_PROPERTIES: {
        u32 mapping;
        u32 filtering;
        u32 rtt;
        pRequest->ExtractPrimitive(&mapping);
        if (pRequest->m_IsError) {
            break;
        }
        pRequest->ExtractPrimitive(&filtering);
        if (pRequest->m_IsError) {
            break;
        }
        pRequest->ExtractPrimitive(&rtt);
        if (pRequest->m_IsError) {
            break;
        }
        ReportNATProperties(mapping, filtering, rtt);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_REPORT_NAT_PROPERTIES);
        break;
    }
    case METHOD_GET_RELAY_PARAMETERS: {
        u32 mode;
        DateTime time;
        String address;
        u16 port;
        u32 addressType;
        u32 serverTime;
        GetRelayParameters(&mode, &time, &address, &port, &addressType, &serverTime);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_GET_RELAY_PARAMETERS);
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&mode), sizeof(mode));
        u64 timeValue = time;
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&timeValue), sizeof(timeValue));
        _Type_string::Add(pResponse, address);
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&port), sizeof(port));
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&addressType), sizeof(addressType));
        pResponse->AppendRaw(reinterpret_cast<const u8*>(&serverTime), sizeof(serverTime));
        break;
    }
    case METHOD_REPORT_NAT_TRAVERSAL_RESULT_DETAIL: {
        u32 cid;
        bool isSucceeded;
        s32 detail;
        u32 rtt;
        pRequest->ExtractPrimitive(&cid);
        if (pRequest->m_IsError) {
            break;
        }
        *pRequest >> isSucceeded;
        if (pRequest->m_IsError) {
            break;
        }
        pRequest->ExtractPrimitiveInline(&detail);
        if (pRequest->m_IsError) {
            break;
        }
        pRequest->ExtractPrimitive(&rtt);
        if (pRequest->m_IsError) {
            break;
        }
        ReportNATTraversalResultDetail(cid, isSucceeded, reinterpret_cast<const NATTraversalResult&>(detail), rtt);
        Protocol::AddMethodID(pResponse, METHOD_RESPONSE_FLAG | METHOD_REPORT_NAT_TRAVERSAL_RESULT_DETAIL);
        break;
    }
    default:
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    *pIsHandled = true;
}

} // namespace nex
} // namespace nn
