#include "nn/pia/inet/inet_NatDetecter.h"
#include "nn/pia/common/common_ErrorHandler.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "pead/peadRandom.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;
// the dummy messages go to this port of the primary server (name is ours)
const u16 DUMMY_MESSAGE_PORT = 33334;
const u16 DUMMY_MESSAGE_NUM = 5;
// the tries to bind to a random port and to find a different port
const u32 BIND_TRY_NUM = 5;
const u16 PORT_TRY_NUM = 100;

// the random ports of the sockets
// 0x00AF5B80
pead::Random s_Random;

// the addresses of the NAT check servers (inline; name is ours)
inline common::SimpleContainer<common::InetAddress, 4>& GetServerAddresses()
{
    return NexFacade::s_pInstance->m_pNatTraverser->m_ServerAddresses;
}

// a random port of the range (inline; name is ours)
inline u16 GetRandomPort()
{
    u16 portMin = NatDetecter::s_PortMin;
    return portMin + s_Random.getU32(NatDetecter::s_PortMax - portMin);
}
} // namespace

// 0x0097FA0A
u16 NatDetecter::s_PortMin = 0xC000;
// 0x0097FA0C
u16 NatDetecter::s_PortMax = 0xFFFF;

// 0x003E2F28 slot 0x20 | slot vf_0x20 of nn::pia::inet::NatDetecter
bool nn::pia::inet::NatDetecter::CheckRetry()
{
    return false;
}

// 0x003E2F30 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatDetecter::OpenSocket()
{
    nn::Result result = m_Socket.Open();
    if (result.IsFailure()) {
        common::ErrorHandler::TraceResult(TRACE_FLAG, result);
        return result;
    }
    if (m_LocalAddress.m_Port != 0) {
        result = m_Socket.Bind(m_LocalAddress);
        if (result.IsSuccess()) {
            return nn::Result();
        }
        common::ErrorHandler::TraceResult(TRACE_FLAG, result);
    }
    for (u32 i = 0; i < BIND_TRY_NUM; i++) {
        m_LocalAddress.m_Port = GetRandomPort();
        nn::Result bindResult = m_Socket.Bind(m_LocalAddress);
        if (bindResult.IsSuccess()) {
            return nn::Result();
        }
        common::ErrorHandler::TraceResult(TRACE_FLAG, bindResult);
        // (a trace of the address; armlink removed the call)
    }
    // (a trace of the address; armlink removed the call)
    result = m_Socket.Close();
    if (result.IsFailure()) {
        common::ErrorHandler::TraceResult(TRACE_FLAG, result);
    }
    return common::RESULT_INVALID_STATE;
}

// 0x003E3078 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatDetecter::CloseSocket()
{
    if (m_Socket.m_OpenNum <= 0) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_Socket.Close();
    if (result.IsFailure()) {
        common::ErrorHandler::TraceResult(TRACE_FLAG, result);
        return result;
    }
    return nn::Result();
}

// 0x003E30D0 (name is ours)
void nn::pia::inet::NatDetecter::GetPredictedAddress(nn::pia::common::InetAddress* pAddress)
{
    const nex::NATCheckMessage& message = m_ReceivedMessages[0].m_Message;
    common::InetAddress address(message.m_Address, static_cast<u16>(message.m_Port));
    u16 increment = static_cast<u16>(NexFacade::s_pInstance->m_pNatTraverser->m_NatProperty.m_PortIncrement);
    if (increment > 1) {
        increment = 1;
    }
    u32 port = address.m_Port + increment;
    if (port > s_PortMax) {
        port = s_PortMin;
    }
    pAddress->m_Address = address.m_Address;
    pAddress->m_Port = port;
    // (the destructor of address is empty)
}

// 0x003E3148 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::AddSendMessage(const nn::pia::inet::NatDetecter::SendNatCheckMessage& message, unsigned short num)
{
    for (u32 i = 0; i < num; i++) {
        SendNatCheckMessage* pMessage = m_SendList.PushBackNew();
        if (pMessage != nullptr) {
            *pMessage = message;
        }
    }
}

// 0x003E3210 (name is ours)
bool nn::pia::inet::NatDetecter::ReceiveMessage()
{
    s32 receivedSize = 0;
    u8 ttl = 0;
    nn::Result result = m_Socket.RecvFrom(reinterpret_cast<u8*>(&m_ReceiveBuffer), sizeof(m_ReceiveBuffer), &m_ReceivedFromAddress, &ttl, &receivedSize);
    if (result.IsFailure()) {
        if (result != common::RESULT_NO_DATA) {
            common::ErrorHandler::TraceResult(TRACE_FLAG, result);
        }
        return false;
    }
    if (receivedSize != sizeof(m_ReceiveBuffer)) {
        return false;
    }
    m_ReceiveBuffer.ToHostByteOrder();
    u32 index = m_ReceiveBuffer.m_Type - nex::NATCheckMessage::TYPE_REPLY_MIN;
    if (index < nex::NATCheckMessage::TYPE_REPLY_NUM) {
        m_ReceivedMessages[index].m_Message = m_ReceiveBuffer;
        m_ReceivedMessages[index].m_Count++;
    }
    return true;
}

// 0x003E32F4 | fefates:bytes [tier B]
u16 nn::pia::inet::NatDetecter::GetPerceivedPort()
{
    return static_cast<u16>(m_ReceivedMessages[0].m_Message.m_Port);
}

// 0x003E3300 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::sendDummyMessage()
{
    const common::InetAddress* pServer = GetPrimaryServerPrimaryPortAddress();
    SendNatCheckMessage message(common::InetAddress(pServer->m_Address, DUMMY_MESSAGE_PORT), 0);
    AddSendMessage(message, DUMMY_MESSAGE_NUM);
}

// 0x003E3458 | fefates:bytes [tier B]
nn::pia::inet::NatDetecter::ReceivedMessage* nn::pia::inet::NatDetecter::GetReceiveMessage(unsigned int index)
{
    return &m_ReceivedMessages[index];
}

// 0x003E3468 | fefates:bytes [tier B]
bool nn::pia::inet::NatDetecter::SendAllQueuedMessage()
{
    while (m_SendList.GetCount() != 0) {
        s32 sentSize = 0;
        SendNatCheckMessage* pMessage = m_SendList.Front();
        m_SendBuffer = pMessage->m_Message;
        m_SendBuffer.ToNetworkByteOrder();
        nn::Result result = m_Socket.SendTo(&m_SendBuffer, sizeof(m_SendBuffer), pMessage->m_Address, &sentSize);
        if (result.IsFailure()) {
            common::ErrorHandler::TraceResult(TRACE_FLAG, result);
            return false;
        }
        pMessage->~SendNatCheckMessage();
        m_SendList.Erase(pMessage);
    }
    return true;
}

// 0x003E3568 | fefates:bytes [tier B]
u16 nn::pia::inet::NatDetecter::GetDifferentPortNumber(unsigned short port)
{
    for (u16 i = 0; i < PORT_TRY_NUM; i++) {
        u16 newPort = GetRandomPort();
        if (newPort != port) {
            return newPort;
        }
    }
    return 0;
}

// 0x003E35E4 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::InitializeReceiveMessage()
{
    for (u32 i = 0; i < nex::NATCheckMessage::TYPE_REPLY_NUM; i++) {
        m_ReceivedMessages[i].m_Count = 0;
        m_ReceivedMessages[i].m_Message.m_Type = 0;
        m_ReceivedMessages[i].m_Message.m_Port = 0;
        m_ReceivedMessages[i].m_Message.m_Address = 0;
        m_ReceivedMessages[i].m_Message.m_Unknown0xC = 0;
    }
}

// 0x003E3618 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::TraceReceivedMessageArray(unsigned long long)
{
    for (u32 i = 0; i < nex::NATCheckMessage::TYPE_REPLY_NUM; i++) {
        // (a trace of the message; armlink removed the call, the empty loop is left)
    }
}

// 0x003E362C | fefates:bytes [tier B]
const nn::pia::common::InetAddress* nn::pia::inet::NatDetecter::GetPrimaryServerPrimaryPortAddress()
{
    common::SimpleContainer<common::InetAddress, 4>& servers = GetServerAddresses();
    if (servers.Begin() == servers.End()) {
        return nullptr;
    }
    return servers.Begin();
}

// 0x003E365C | fefates:bytes [tier B]
const nn::pia::common::InetAddress* nn::pia::inet::NatDetecter::GetPrimaryServerSecondaryPortAddress()
{
    common::SimpleContainer<common::InetAddress, 4>& servers = GetServerAddresses();
    common::InetAddress* it = servers.Begin();
    if (it != servers.End()) {
        it++;
        if (it != servers.End()) {
            return it;
        }
    }
    return nullptr;
}

// 0x003E3698 slot 0x0C | fefates:bytes
void nn::pia::inet::NatDetecter::Cleanup()
{
    m_SendList.ClearNodes();
    if (m_Socket.m_OpenNum > 0) {
        nn::Result result = m_Socket.Close();
        if (result.IsFailure()) {
            common::ErrorHandler::TraceResult(TRACE_FLAG, result);
        }
    }
}

// 0x003E375C slot 0x08 | fefates:callseq
nn::Result nn::pia::inet::NatDetecter::Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress& localAddress)
{
    m_RetryCount = 0;
    m_LocalAddress = localAddress;
    if (GetServerAddresses().GetNum() == 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_SendList.ClearNodes();
    return nn::Result();
}

// 0x003E37F0 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatDetecter::RetryBind()
{
    nn::Result result = m_Socket.Close();
    if (result.IsSuccess()) {
        result = m_Socket.Open();
        if (result.IsSuccess()) {
            for (u32 i = 0; i < BIND_TRY_NUM; i++) {
                m_LocalAddress.m_Port = GetRandomPort();
                nn::Result bindResult = m_Socket.Bind(m_LocalAddress);
                if (bindResult.IsSuccess()) {
                    break;
                }
                common::ErrorHandler::TraceResult(TRACE_FLAG, bindResult);
                // (a trace of the address; armlink removed the call)
            }
            return nn::Result();
        }
    }
    common::ErrorHandler::TraceResult(TRACE_FLAG, result);
    return result;
}

// 0x003E38D0 | fefates:bytes [tier B]
nn::pia::inet::NatDetecter::NatDetecter()
{
    // the members construct themselves (the retry count and the RTT are not initialized)
}

// 0x003E3A08 | fefates:bytes
// 0x003E39C0 (deleting dtor)
nn::pia::inet::NatDetecter::~NatDetecter()
{
    // the members are destroyed (their destructors are empty but the ones of the job and the
    // socket)
}

// 0x004296DC slot 0x10 | fefates:bytes
void nn::pia::inet::NatDetecter::Retry()
{
    m_RetryCount++;
    m_SendList.ClearNodes();
}

// 0x004288B8 slot 0x28 | slot vf_0x28 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatDetecter::StartDetectionJob()
{
    m_DetectionJob.Ready(true);
}

// 0x00427438 slot 0x2C | fefates:bytes
void nn::pia::inet::NatDetecter::CancelDetectionJob()
{
    m_DetectionJob.m_IsCancelRequested = true;
    m_DetectionJob.WaitForCompletion(2);
}

// 0x003E35E0
// 0x003E35DC (deleting dtor)
nn::pia::inet::NatDetecter::SendNatCheckMessageList::~SendNatCheckMessageList()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
