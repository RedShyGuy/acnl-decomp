#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
// the response could not be read
const int RESULT_INVALID_RESPONSE = static_cast<int>(0x8001000F);
// there is no server protocol for a call over the local loopback
const int RESULT_NO_PROTOCOL = static_cast<int>(0x80010002);
// the end points of these principals (and those without the function) get trusted messages
const u32 SPECIAL_PRINCIPAL_FIRST = 110;
const u32 SPECIAL_PRINCIPAL_COUNT = 10;
const u32 FUNCTION_UNKNOWN_08000000 = 0x08000000;
} // namespace

// 0x00370A18 | fefates:bytes [tier B]
void nn::nex::ClientProtocol::ProcessResponse(nn::nex::Message* pMessage, nn::nex::EndPoint* pEndPoint)
{
    if (!m_IsEndPointKept || pEndPoint != nullptr) {
        m_pEndPoint = pEndPoint;
        m_IsEndPointKept = false;
        if (pEndPoint == nullptr) {
            m_PrincipalId = 0;
            m_ConnectionId = 0;
        } else {
            m_PrincipalId = pEndPoint->m_PrincipalId;
            m_ConnectionId = pEndPoint->m_ConnectionId;
        }
    }
    if (pEndPoint != nullptr
        && (pEndPoint->m_PrincipalId - SPECIAL_PRINCIPAL_FIRST < SPECIAL_PRINCIPAL_COUNT
            || (pEndPoint->GetSupportedFunctionsFlags() & FUNCTION_UNKNOWN_08000000) == 0)) {
        pMessage->m_Unknown0x3B = true;
    }
    qResult result;
    ExtractCallOutcome(pMessage, &result);
    ProtocolCallContext* pContext = static_cast<ProtocolCallContext*>(RetrieveCallContext(pMessage));
    if (pContext == nullptr) {
        return;
    }
    if (pMessage->m_IsError) {
        pContext->SignalFailure(qResult(RESULT_INVALID_RESPONSE));
        return;
    }
    if (!result) {
        pContext->SignalFailure(result);
        return;
    }
    ExtractCallSpecificResults(pMessage, pContext);
    if (pMessage->m_IsError) {
        pContext->SignalFailure(qResult(RESULT_INVALID_RESPONSE));
        return;
    }
    pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
}

// 0x00370C88 | fefates:bytes [tier B]
bool nn::nex::ClientProtocol::SendOverLocalLoopback(nn::nex::ProtocolCallContext* pContext, nn::nex::Message* pMessage)
{
    Protocol* pServer = m_pBroker->FindProtocol(m_ProtocolId);
    if (pServer == nullptr) {
        pContext->SignalFailure(qResult(RESULT_NO_PROTOCOL));
        return false;
    }
    Message request(pMessage->GetBuffer());
    u8 key;
    request.ExtractPrimitive(&key);
    if ((key & SHORT_PROTOCOL_ID) == SHORT_PROTOCOL_ID) {
        u16 protocolId;
        request.ExtractPrimitive(&protocolId);
    }
    qResult result = pServer->m_pBroker->ProcessLocalCall(&request, this, pServer, 0);
    if (result) {
        return true;
    }
    if (pContext->m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        pContext->SignalFailure(result);
    }
    return false;
}

// 0x00370DD4 | fefates:bytes [tier B]
void nn::nex::ClientProtocol::SetDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    if (m_pDefaultCredentials != nullptr) {
        m_pDefaultCredentials->ReleaseRef();
        m_pDefaultCredentials = nullptr;
    }
    if (pCredentials != nullptr) {
        m_pDefaultCredentials = pCredentials;
        pCredentials->AcquireRef();
    }
}

// 0x00370E60
void nn::nex::ClientProtocol::vf_0x10()
{
    // nothing to do in this build
}

// 0x00370E64 | fefates:bytes [tier B]
nn::nex::ClientProtocol::ClientProtocol(unsigned int flags) : Protocol(flags), m_pDefaultCredentials(nullptr)
{
}

// 0x00370EF0 | fefates:bytes [tier B]
// 0x00370EDC (deleting dtor)
nn::nex::ClientProtocol::~ClientProtocol()
{
    if (m_pDefaultCredentials != nullptr) {
        m_pDefaultCredentials->ReleaseRef();
        m_pDefaultCredentials = nullptr;
    }
}

// 0x003D5828 | fefates:callgraph [tier C]
void nn::nex::ClientProtocol::SetCallError(nn::nex::qResult result)
{
    // (the trace of the error is not in this build)
}

// 0x0072AE5C
nn::nex::Protocol::_Type nn::nex::ClientProtocol::GetProtocolType() const
{
    return TYPE_CLIENT;
}

// 0x0072AE64
const wchar_t* nn::nex::ClientProtocol::GetType() const
{
    return L"ClientProtocol";
}

// 0x0072AE8C
bool nn::nex::ClientProtocol::IsAKindOf(const wchar_t* pType) const
{
    return wcscmp(L"ClientProtocol", pType) == 0 || wcscmp(L"Protocol", pType) == 0 || wcscmp(L"SystemComponent", pType) == 0;
}

} // namespace nex
} // namespace nn
