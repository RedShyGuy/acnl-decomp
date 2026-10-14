#include "nn/nex/nex_ServerProtocol.h"
#include "nn/nex/nex_CallProtocolMethodOperation.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SecurityContextManager.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the end points of these principals (and those without the function) get trusted messages
const u32 SPECIAL_PRINCIPAL_FIRST = 110;
const u32 SPECIAL_PRINCIPAL_COUNT = 10;
const u32 FUNCTION_UNKNOWN_08000000 = 0x08000000;
} // namespace

// 0x00374230 | mk7dlp:callseq-callee [tier A]
void nn::nex::ServerProtocol::SetCallError(nn::nex::qResult result)
{
    // (the current operation is the call being dispatched)
    CallProtocolMethodOperation* pOperation = static_cast<CallProtocolMethodOperation*>(m_pBroker->m_OperationManager.GetCurrentOperation());
    pOperation->m_Result = result;
}

// 0x00374264 | fefates:callseq [tier C]
void nn::nex::ServerProtocol::DispatchCallRequest(unsigned int callId, nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, int* pAttemptCount, nn::nex::EndPoint* pEndPoint)
{
    if (pEndPoint != nullptr) {
        u32 address = pEndPoint->m_URL.GetInetAddress()->GetAddress();
        if (address == 0) {
            address = SecurityContextManager::GetInstance()->GetCurrentAddress();
        }
        SecurityContextManager::GetInstance()->Push(pEndPoint->m_PrincipalId, pEndPoint->m_ConnectionId, address);
        m_pEndPoint = pEndPoint;
        m_IsEndPointKept = false;
        m_PrincipalId = pEndPoint->m_PrincipalId;
        m_ConnectionId = pEndPoint->m_ConnectionId;
        if (pEndPoint->m_PrincipalId - SPECIAL_PRINCIPAL_FIRST < SPECIAL_PRINCIPAL_COUNT
            || (pEndPoint->GetSupportedFunctionsFlags() & FUNCTION_UNKNOWN_08000000) == 0) {
            pRequest->m_IsWithoutStructureHeader = true;
            pResponse->m_IsWithoutStructureHeader = true;
        }
    } else {
        u32 address = SecurityContextManager::GetInstance()->GetCurrentAddress();
        SecurityContextManager::GetInstance()->Push(m_PrincipalId, m_ConnectionId, address);
    }
    if (m_Flags & FLAG_NO_LOCK) {
        if (UseAttemptCountMethod()) {
            DispatchProtocolMessageWithAttemptCount(callId, pRequest, pResponse, pIsHandled, pAttemptCount, pEndPoint);
        } else {
            DispatchProtocolMessage(pRequest, pResponse, pIsHandled, pEndPoint);
        }
    } else {
        ScopedCS lock(Scheduler::GetInstance()->m_Lock);
        if (UseAttemptCountMethod()) {
            DispatchProtocolMessageWithAttemptCount(callId, pRequest, pResponse, pIsHandled, pAttemptCount, pEndPoint);
        } else {
            DispatchProtocolMessage(pRequest, pResponse, pIsHandled, pEndPoint);
        }
    }
    if (!m_IsEndPointKept) {
        m_pEndPoint = nullptr;
        m_PrincipalId = 0;
        m_ConnectionId = 0;
    }
    SecurityContextManager::GetInstance()->Pop();
    if (m_Flags & FLAG_UNKNOWN_8) {
        *pIsHandled = false;
    }
}

// 0x003744D4
bool nn::nex::ServerProtocol::UseAttemptCountMethod()
{
    return false;
}

// 0x003744DC
void nn::nex::ServerProtocol::vf_0x10()
{
    // nothing to do in this build
}

// 0x003744E0
void nn::nex::ServerProtocol::DispatchProtocolMessageWithAttemptCount(unsigned int callId, nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, int* pAttemptCount, nn::nex::EndPoint* pEndPoint)
{
    // nothing to do in this build
}

// 0x003744E4 | fefates:bytes [tier B]
nn::nex::ServerProtocol::ServerProtocol(unsigned int flags) : Protocol(flags)
{
}

// 0x0037E478 | fefates:callgraph [tier C]
// 0x00374558 (deleting dtor)
nn::nex::ServerProtocol::~ServerProtocol()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B348
nn::nex::Protocol::_Type nn::nex::ServerProtocol::GetProtocolType() const
{
    return TYPE_SERVER;
}

// 0x0072B350
const wchar_t* nn::nex::ServerProtocol::GetType() const
{
    return L"ServerProtocol";
}

// 0x0072B378
bool nn::nex::ServerProtocol::IsAKindOf(const wchar_t* pType) const
{
    return wcscmp(L"ServerProtocol", pType) == 0 || wcscmp(L"Protocol", pType) == 0 || wcscmp(L"SystemComponent", pType) == 0;
}

} // namespace nex
} // namespace nn
