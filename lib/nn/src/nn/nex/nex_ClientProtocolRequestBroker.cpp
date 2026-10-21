#include "nn/nex/nex_ClientProtocolRequestBroker.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_CallProtocolMethodOperation.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallObserver.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_ServerProtocol.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
namespace {
// (the client broker has no protocols of its own and no local calls)
const int RESULT_NOT_IMPLEMENTED = 0x80010002;
// the protocol whose missing server protocol is not answered (name is ours)
const u16 PROTOCOL_ID_UNANSWERED = 1;
} // namespace

// 0x003BCBD0
// 0x003BCBAC (deleting dtor)
nn::nex::ClientProtocolRequestBroker::~ClientProtocolRequestBroker()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BC368 slot 0x08 | fefates:bytes
void nn::nex::ClientProtocolRequestBroker::ProcessMessage(nn::nex::CallProtocolMethodOperation* pOperation, nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pBuffer)
{
    m_pCurrentEndPoint = pEndPoint;
    ProcessMessageCore(pOperation, pEndPoint, pBuffer);
    m_pCurrentEndPoint = nullptr;
}

// 0x003BC4B8 slot 0x0C | slot vf_0x0C of nn::nex::ProtocolRequestBrokerInterface
void nn::nex::ClientProtocolRequestBroker::ProcessCallRequest(unsigned short protocolId, nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint)
{
    ServerProtocol* pProtocol = RVClientCore::GetInstance()->FindProtocol(protocolId);
    if (pProtocol == nullptr) {
        pProtocol = FindProtocol(protocolId);
        if (pProtocol == nullptr && protocolId == PROTOCOL_ID_UNANSWERED) {
            *pIsHandled = false;
            return;
        }
    }
    u32 callId = Protocol::ExtractCallID(pRequest);
    Message results;
    *pIsHandled = false;
    if (pProtocol != nullptr) {
        ScopedCall call(m_CallPolicy);
        int attemptCount = 0;
        CallProtocolMethodOperation* pOperation = static_cast<CallProtocolMethodOperation*>(m_OperationManager.GetCurrentOperation());
        pProtocol->DispatchCallRequest(pOperation->m_AttemptCount, pRequest, &results, pIsHandled, &attemptCount, pEndPoint);
    } else {
        // no protocol: the call fails
        *pIsHandled = true;
        CallProtocolMethodOperation* pOperation = static_cast<CallProtocolMethodOperation*>(m_OperationManager.GetCurrentOperation());
        pOperation->m_Result = qResult(RESULT_NOT_IMPLEMENTED);
    }
    if (*pIsHandled) {
        Protocol::AddProtocolKey(pResponse, Protocol::COMMAND_RESPONSE, protocolId);
        Protocol::AddCallResponse(pResponse, static_cast<CallProtocolMethodOperation*>(m_OperationManager.GetCurrentOperation()), callId, results);
    }
}

// 0x003BC9B4 slot 0x10 | fefates:bytes
void nn::nex::ClientProtocolRequestBroker::ProcessCallResponse(unsigned short protocolId, nn::nex::Message* pResponse, nn::nex::EndPoint* pEndPoint)
{
    ClientProtocol* pProtocol = static_cast<ClientProtocol*>(RVClientCore::GetInstance()->m_pProtocolRegistry->GetProtocol(protocolId, Protocol::TYPE_CLIENT));
    if (pProtocol != nullptr) {
        pProtocol->ProcessResponse(pResponse, pEndPoint);
    }
}

// 0x003BC3F0 slot 0x14 | slot vf_0x14 of nn::nex::ProtocolRequestBrokerInterface
bool nn::nex::ClientProtocolRequestBroker::RegisterProtocol(nn::nex::Protocol*)
{
    return false;
}

// 0x003BC9B0 slot 0x18 | slot vf_0x18 of nn::nex::ProtocolRequestBrokerInterface
void nn::nex::ClientProtocolRequestBroker::UnregisterProtocol(nn::nex::Protocol*)
{
    // nothing to do: the client broker has no protocols of its own
}

// 0x003BC308 slot 0x1C | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
bool nn::nex::ClientProtocolRequestBroker::vf_0x1C()
{
    return false;
}

// 0x003BC310 slot 0x20 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
bool nn::nex::ClientProtocolRequestBroker::vf_0x20()
{
    return false;
}

// 0x003BC9F4 slot 0x24 (name is ours)
bool nn::nex::ClientProtocolRequestBroker::RegisterServerProtocol(unsigned short id, nn::nex::ServerProtocol* pProtocol)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    if (m_ServerProtocols.find(id) != m_ServerProtocols.end()) {
        return false;
    }
    m_ServerProtocols[id] = pProtocol;
    return true;
}

// 0x003BCADC slot 0x28 (name is ours)
bool nn::nex::ClientProtocolRequestBroker::UnregisterServerProtocol(unsigned short id)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    qMap<u16, ServerProtocol*>::iterator it = m_ServerProtocols.find(id);
    if (it != m_ServerProtocols.end()) {
        m_ServerProtocols.erase(it);
    }
    return true;
}

// 0x003BC3F8 slot 0x2C (name is ours)
nn::nex::ServerProtocol* nn::nex::ClientProtocolRequestBroker::FindProtocol(unsigned short id)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    qMap<u16, ServerProtocol*>::iterator it = m_ServerProtocols.find(id);
    if (it == m_ServerProtocols.end()) {
        return nullptr;
    }
    return it->second;
}

// 0x003BC384 slot 0x30 (name is ours)
nn::nex::qResult nn::nex::ClientProtocolRequestBroker::ProcessLocalCall(nn::nex::Message* pMessage, nn::nex::Protocol* pClient, nn::nex::Protocol* pServer, u32 unknown)
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003BCAC0 slot 0x34 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
nn::nex::qResult nn::nex::ClientProtocolRequestBroker::vf_0x34()
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003BCB90 slot 0x38 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
nn::nex::qResult nn::nex::ClientProtocolRequestBroker::vf_0x38()
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003BC3A0 slot 0x3C (name is ours)
void nn::nex::ClientProtocolRequestBroker::PeerDisconnected(nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    Core::GetInstance()->m_pCallContextRegister->CancelCallContexts(pEndPoint, pEndPoint->m_ConnectionId);
}

// 0x003BC318 slot 0x40 (name is ours)
void nn::nex::ClientProtocolRequestBroker::FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    Core::GetInstance()->m_pCallContextRegister->CancelCallContexts(pEndPoint, pEndPoint->m_ConnectionId);
}

// 0x003BC634 | fefates:callgraph [tier C]
void nn::nex::ClientProtocolRequestBroker::ProcessMessageCore(nn::nex::CallProtocolMethodOperation* pOperation, nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pBuffer)
{
    Message message(pBuffer);
    Protocol::_Command command;
    u16 protocolId;
    Protocol::ExtractProtocolKey(&message, command, protocolId);
    m_BandwidthCounters[protocolId]->m_Incoming += pBuffer->m_ContentSize;
    if (command == Protocol::COMMAND_REQUEST) {
        pOperation->InitializeForNewCallAttempt();
        m_OperationManager.OperationBegins(pOperation);
        {
            Message response;
            bool isHandled = false;
            ProtocolCallObserver::NotifyCallBegins();
            ProcessCallRequest(protocolId, &message, &response, &isHandled, pEndPoint);
            ProtocolCallObserver::NotifyCallEnds(pEndPoint);
            if (!pOperation->m_Unknown0x14 && isHandled) {
                Buffer* pResponseBuffer = response.GetBuffer();
                {
                    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
                    pEndPoint->_Send(pResponseBuffer, 1, false, 0, 0);
                }
                m_BandwidthCounters[protocolId]->m_Outgoing += pResponseBuffer->m_ContentSize;
            }
        }
        m_OperationManager.OperationEnds(pOperation);
    } else {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        ProcessCallResponse(protocolId, &message, pEndPoint);
    }
    if (message.m_IsError) {
        // (the trace of the text is removed)
        StringStream stream;
        stream << L"ClientProtocolRequestBroker: Error processing message. pBuffer=" << static_cast<const void*>(pBuffer) << L" Protocol ID=" << static_cast<int>(protocolId)
               << L" CntSize=" << pBuffer->m_ContentSize << L" Size=" << pBuffer->m_Size - pBuffer->m_HeaderSize;
        if (pEndPoint != nullptr) {
            stream << L"  EndPoint: CID=" << pEndPoint->m_ConnectionId << L" PID=" << pEndPoint->m_PrincipalId << L" URL=" << pEndPoint->m_URL;
        } else {
            stream << L"  <EndPoint is NULL>";
        }
    }
}

// 0x003CE4B4 (name is ours)
template <>
DECOMP_NOINLINE qMap<u16, ServerProtocol*>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

// 0x003CE550 | fefates:callgraph [tier C]
template <>
DECOMP_NOINLINE qMap<u16, ServerProtocol*>::~qMap()
{
    // nothing to do: the tree is destroyed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0082EC28 (name is ours)
template <>
DECOMP_NOINLINE nn::nex::ServerProtocol*& map<u16, nn::nex::ServerProtocol*, less<u16>, nn::nex::MemAllocator<pair<const u16, nn::nex::ServerProtocol*> > >::operator[](const u16& key)
{
    return IndexInline(key);
}
} // namespace std

namespace __rw {
// the functions of the tree of the server protocols
// 0x0080B528 (name is ours)
template __rb_tree<u16, std::pair<const u16, nn::nex::ServerProtocol*>, __select1st<std::pair<const u16, nn::nex::ServerProtocol*>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::ServerProtocol*> > >::iterator __rb_tree<u16, std::pair<const u16, nn::nex::ServerProtocol*>, __select1st<std::pair<const u16, nn::nex::ServerProtocol*>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::ServerProtocol*> > >::erase(iterator it);
// 0x0080BB88 (name is ours)
template void __rb_tree<u16, std::pair<const u16, nn::nex::ServerProtocol*>, __select1st<std::pair<const u16, nn::nex::ServerProtocol*>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::ServerProtocol*> > >::_C_erase(Node* pNode);
// 0x0080BBC4 (name is ours)
template __rb_tree<u16, std::pair<const u16, nn::nex::ServerProtocol*>, __select1st<std::pair<const u16, nn::nex::ServerProtocol*>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::ServerProtocol*> > >::iterator __rb_tree<u16, std::pair<const u16, nn::nex::ServerProtocol*>, __select1st<std::pair<const u16, nn::nex::ServerProtocol*>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::ServerProtocol*> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const u16, nn::nex::ServerProtocol*>& value);
} // namespace __rw
