#include "nn/nex/nex_JobDetectNATType.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/socket/socket_Api.h"

#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the results of the detection (the names are ours)
const int RESULT_DETECTION_SUCCESS = 0x10001;
const int RESULT_FAILURE = static_cast<int>(0x80010001);
// the types of the requests (the replies come back with the same type)
const u32 TYPE_ADDRESS = 0;
const u32 TYPE_MAPPING = 101;
const u32 TYPE_MAPPING_PORT = 102;
const u32 TYPE_FILTERING = 103;
// the tests of MODE_TESTS (m_TestState)
const u32 TEST_NONE = 0;
const u32 TEST_MAPPING = 1;
const u32 TEST_FILTERING_PORT = 2;
const u32 TEST_FILTERING_ADDRESS = 3;
const u32 TEST_MAPPING_ADDRESS = 4;
const u32 TEST_MAPPING_PORT = 5;
// the indices of g_NATTypeNames
const int NAME_UNKNOWN = 0;
const int NAME_ENDPOINT_INDEPENDENT = 1;
const int NAME_ADDRESS_DEPENDENT = 2;
const int NAME_ADDRESS_AND_PORT_DEPENDENT = 3;
// the port of the address requests and the number of the requests of each type
const u16 NAT_CHECK_PORT = 33334;
const int REQUEST_COUNT = 5;
// the time for the replies and the first round trip time of the tests (ms)
const u32 REPLY_TIME = 2500;
// the shortest time for a test reply (ms)
const u32 MIN_TEST_REPLY_TIME = 625;
// the times between the tests of the waiting steps (ms)
const int RECEIVE_WAIT_TIME = 100;
const int TEST_WAIT_TIME = 16;
// the attempts of the requests and of the tests
const u32 MAX_RETRY_COUNT = 3;
const u32 MAX_PREDICTION_RETRY_COUNT = 4;
const u32 MAX_TEST_RETRY_COUNT = 3;
// the attempts to bind the socket to a port of the root transport
const u32 BIND_ATTEMPTS = 5;
// the values of NATProperties
const u8 NAT_MAPPING_INDEPENDENT = 1;
const u8 NAT_MAPPING_DEPENDENT = 2;
const u8 NAT_FILTERING_INDEPENDENT = 1;
const u8 NAT_FILTERING_DEPENDENT = 2;
const u16 MAX_PORT = 0xFFFF;

// (inline) the name is one of a known type
inline bool IsKnownType(const wchar_t* pName)
{
    return wcscmp(pName, g_NATTypeNames[NAME_ENDPOINT_INDEPENDENT]) == 0 || wcscmp(pName, g_NATTypeNames[NAME_ADDRESS_DEPENDENT]) == 0 ||
           wcscmp(pName, g_NATTypeNames[NAME_ADDRESS_AND_PORT_DEPENDENT]) == 0;
}

// (inline) the name is one of a type that depends on the destination
inline bool IsDependentType(const wchar_t* pName)
{
    return wcscmp(pName, g_NATTypeNames[NAME_ADDRESS_DEPENDENT]) == 0 || wcscmp(pName, g_NATTypeNames[NAME_ADDRESS_AND_PORT_DEPENDENT]) == 0;
}

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// (inline) the call of the caller (null if it was cancelled)
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// the public port of the last port prediction (name is ours)
// 0x0096CE24
u32 g_LastPublicPort;
// (names are ours)
// 0x0096CE2C
const wchar_t* g_NATTypeNames[4] = {L"Unknown (u)", L"Endpoint-Independent (ei)", L"Address-Dependent (ad)", L"Address and Port-Dependent (apd)"};

// 0x0082FC58
template <>
DECOMP_NOINLINE void qList<JobDetectNATType::Request>::push_back(const JobDetectNATType::Request& value)
{
    PushBackInline(value);
}

// 0x0082F21C | std::list::remove
template <>
DECOMP_NOINLINE void qList<InetAddress>::remove(const InetAddress& value)
{
    Node* pHead = m_pHead;
    for (Node* pNode = pHead->m_pNext; pNode != pHead;) {
        Node* pNext = pNode->m_pNext;
        if (pNode->m_Value == value) {
            erase(pNode);
        }
        pNode = pNext;
    }
}

// 0x00389C54 (name is ours)
void nn::nex::JobDetectNATType::Start()
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        SetToComplete();
        return;
    }
    if (GetRootTransport() != nullptr && GetRootTransport()->m_AreNATCheckURLsReady) {
        u8 address[4];
        u8 netmask[4];
        nn::socket::GetPrimaryAddress(address, netmask);
        if ((address[0] | address[1] | address[2] | address[3]) != 0 && m_Mode != 0) {
            qVector<StringStream*>& urls = GetRootTransport()->m_NATCheckURLs;
            for (StringStream** ppURL = urls.m_pBegin; ppURL < urls.m_pEnd; ppURL++) {
                if ((*ppURL)->GetLength() == 0) {
                    continue;
                }
                StationURL url((*ppURL)->CStr());
                m_Servers.PushBackInline(*url.GetInetAddress());
            }
            if (m_Servers.GetSize() != 0) {
                m_Socket.Open(true);
                bool isBound = m_LocalAddress.GetPortNumber() != 0 && m_Socket.Bind(&m_LocalAddress, false);
                for (u32 i = 0; !isBound && i < BIND_ATTEMPTS; i++) {
                    m_LocalAddress.SetPortNumber(RootTransport::GetNextPortNumber());
                    isBound = m_Socket.Bind(&m_LocalAddress, false);
                }
                if (isBound) {
                    if (m_Mode == MODE_PORT_PREDICTION || m_Mode == MODE_PROPERTIES) {
                        SetStep(Step(&JobDetectNATType::SendRequests, ""));
                    } else if (m_Mode == MODE_TESTS) {
                        SetStep(Step(&JobDetectNATType::StartTests, ""));
                    }
                    return;
                }
                m_Socket.Close();
            }
        }
    }
    pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
    SetToComplete();
}

// 0x0038B16C (name is ours)
void nn::nex::JobDetectNATType::SendRequests()
{
    if (GetRootTransport() == nullptr) {
        CallContext* pContext = GetCallContext(m_CallContextId);
        if (pContext != nullptr) {
            pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
        }
        SetToComplete();
        return;
    }
    for (u32 i = 0; i < NATCheckMessage::TYPE_REPLY_NUM; i++) {
        m_Replies[i] = Reply();
    }
    m_ReplyDeadline = Time::ConvertTimeoutToDeadline(REPLY_TIME);
    for (int i = 0; i < REQUEST_COUNT; i++) {
        InetAddress server(m_Servers.front().GetAddress(), NAT_CHECK_PORT);
        m_Requests.push_back(Request(server, TYPE_ADDRESS));
    }
    // a NAT that maps per destination is asked through the second server
    qList<InetAddress>::Node* pSecond = m_Servers.GetFirst();
    bool usesSecond = !m_Servers.IsEnd(pSecond) && !m_Servers.IsEnd(pSecond = pSecond->m_pNext) && m_Mode == MODE_PORT_PREDICTION &&
                      GetRootTransport()->GetNATProperties().GetNATMapping() == NAT_MAPPING_DEPENDENT;
    if (usesSecond) {
        for (int i = 0; i < REQUEST_COUNT; i++) {
            m_Requests.push_back(Request(pSecond->m_Value, TYPE_MAPPING));
        }
    } else {
        for (int i = 0; i < REQUEST_COUNT; i++) {
            m_Requests.push_back(Request(m_Servers.front(), TYPE_MAPPING));
        }
    }
    if (m_Mode == MODE_PROPERTIES) {
        qList<InetAddress>::Node* pFirst = m_Servers.GetFirst();
        for (int i = 0; i < REQUEST_COUNT; i++) {
            m_Requests.push_back(Request(pFirst->m_Value, TYPE_MAPPING_PORT));
        }
        // the filtering: a server with another address and the same port
        for (qList<InetAddress>::Node* pNode = pFirst->m_pNext; !m_Servers.IsEnd(pNode); pNode = pNode->m_pNext) {
            if (pNode->m_Value.GetAddress() != m_Servers.front().GetAddress() && pNode->m_Value.GetPortNumber() == m_Servers.front().GetPortNumber()) {
                for (int i = 0; i < REQUEST_COUNT; i++) {
                    m_Requests.push_back(Request(pNode->m_Value, TYPE_FILTERING));
                }
                break;
            }
        }
    }
    SetStep(Step(&JobDetectNATType::ReceiveReplies, ""));
}

// 0x0038ACB0 (name is ours)
void nn::nex::JobDetectNATType::ReceiveReplies()
{
    if (Time::GetTime() < m_ReplyDeadline && !m_HasReplies) {
        // the requests are sent until one fails
        while (m_Requests.GetSize() != 0) {
            Request& request = m_Requests.front();
            bool isSent = m_Socket.SendTo(reinterpret_cast<u8*>(&request.m_Message), sizeof(NATCheckMessage), &request.m_Address) == Socket::RESULT_SUCCESS;
            m_Requests.erase(m_Requests.GetFirst());
            if (!isSent) {
                break;
            }
        }
        u32 size = 0;
        if (m_Socket.RecvFrom(reinterpret_cast<u8*>(&m_Received), sizeof(m_Received), &size, &m_ReplyAddress) == Socket::RESULT_SUCCESS && size == sizeof(m_Received)) {
            m_Received.ToHostByteOrder();
            u32 index = m_Received.m_Type - NATCheckMessage::TYPE_REPLY_MIN;
            if (index < NATCheckMessage::TYPE_REPLY_NUM) {
                m_Replies[index].m_Message = m_Received;
                m_Replies[index].m_Count++;
            }
            if (m_Mode == MODE_PORT_PREDICTION) {
                if (m_Replies[0].m_Count != 0) {
                    m_HasReplies = true;
                }
            } else if (m_Mode == MODE_PROPERTIES) {
                if (m_Replies[0].m_Count != 0 && m_Replies[1].m_Count != 0 && m_Replies[2].m_Count != 0) {
                    m_HasReplies = true;
                }
            }
        }
        SetToWaiting(RECEIVE_WAIT_TIME);
        return;
    }
    u32 port = m_Replies[0].m_Message.m_Port;
    if (m_Mode == MODE_PORT_PREDICTION) {
        if (port == 0 && m_RetryCount < MAX_PREDICTION_RETRY_COUNT) {
            m_RetryCount++;
            m_HasReplies = false;
            SetStep(Step(&JobDetectNATType::SendRequests, ""));
            return;
        }
        if (port < g_LastPublicPort && m_RequestsRelayProbe && m_RetryCount < MAX_PREDICTION_RETRY_COUNT) {
            // the ports go down: the next port of the root transport
            m_Socket.Close();
            m_Socket.Open(true);
            for (u32 i = 0; i < BIND_ATTEMPTS; i++) {
                m_LocalAddress.SetPortNumber(RootTransport::GetNextPortNumber());
                if (m_Socket.Bind(&m_LocalAddress, false)) {
                    break;
                }
            }
            m_RetryCount++;
            m_HasReplies = false;
            SetStep(Step(&JobDetectNATType::SendRequests, ""));
            return;
        }
        g_LastPublicPort = port;
        SetStep(Step(&JobDetectNATType::PredictPort, ""));
        return;
    }
    if (port != 0 && m_Replies[2].m_Message.m_Port != 0) {
        SetStep(Step(&JobDetectNATType::Finish, ""));
        return;
    }
    if (m_RetryCount < MAX_RETRY_COUNT) {
        m_RetryCount++;
        m_HasReplies = false;
        SetStep(Step(&JobDetectNATType::SendRequests, ""));
        return;
    }
    SetStep(Step(&JobDetectNATType::Finish, ""));
}

// 0x0038A5DC (name is ours)
void nn::nex::JobDetectNATType::PredictPort()
{
    InetAddress address(m_Replies[0].m_Message.m_Address, static_cast<u16>(m_Replies[0].m_Message.m_Port));
    m_PredictedURL.SetInetAddress(&address);
    u32 increment = GetRootTransport()->GetNATProperties().m_PortIncrement;
    u16 port = m_PredictedURL.GetPortNumber() + increment;
    if (port >= MAX_PORT) {
        port = MAX_PORT;
    }
    m_PredictedURL.SetPortNumber(port);
    if (m_RequestsRelayProbe) {
        qList<StationURL> targets;
        targets.PushBackInline(m_TargetURL);
        m_pRelayInterface->RelayProbeRequest(targets, m_PredictedURL);
    }
    SetStep(Step(&JobDetectNATType::Finish, ""));
}

// 0x0038A7AC (name is ours)
bool nn::nex::JobDetectNATType::UpdateNATProperties()
{
    if (GetRootTransport() == nullptr) {
        return false;
    }
    u32 port = m_Replies[0].m_Message.m_Port;
    if (port == 0 || m_Replies[2].m_Message.m_Port == 0) {
        return false;
    }
    m_Properties.SetPrivateAddress(m_LocalAddress.GetAddressStr());
    m_Properties.m_PrivatePort = m_LocalAddress.GetPortNumber();
    m_PublicAddress.SetAddress(m_Replies[2].m_Message.m_Address);
    m_PublicAddress.SetPortNumber(m_Replies[2].m_Message.m_Port);
    m_Properties.SetPublicAddress(m_PublicAddress.GetAddressStr());
    m_Properties.m_PublicPort = m_PublicAddress.GetPortNumber();
    m_Properties.m_NatFiltering = m_Replies[1].m_Message.m_Port != 0 ? NAT_FILTERING_INDEPENDENT : NAT_FILTERING_DEPENDENT;
    if (m_Replies[0].m_Message.m_Port == m_Replies[2].m_Message.m_Port) {
        m_Properties.m_NatMapping = NAT_MAPPING_INDEPENDENT;
    } else {
        m_Properties.m_NatMapping = NAT_MAPPING_DEPENDENT;
        u32 first = m_Replies[0].m_Message.m_Port;
        u32 last = m_Replies[2].m_Message.m_Port;
        m_Properties.m_PortIncrement = first > last ? first - last : last - first;
        if (m_LocalAddress.GetPortNumber() == m_Replies[2].m_Message.m_Port) {
            m_Properties.m_IsPortPreserved = true;
            m_Properties.m_PortIncrement = 1;
        }
    }
    GetRootTransport()->SetNATProperties(m_Properties);
    return true;
}

// 0x0038A95C (name is ours)
void nn::nex::JobDetectNATType::Finish()
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        SetToComplete();
        return;
    }
    if (GetRootTransport() == nullptr) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
        SetToComplete();
        return;
    }
    m_Socket.Close();
    bool isSuccess;
    switch (m_Mode) {
    case MODE_PROPERTIES:
        isSuccess = UpdateNATProperties();
        break;
    case MODE_TESTS:
        if (m_RunsTwice) {
            // the tests run a second time
            m_Servers.ClearInline();
            m_LocalAddress.SetAddress(L"");
            m_LocalAddress.SetPortNumber(0);
            m_TestRetryCount = 0;
            m_RoundTripTime = 0;
            m_TestState = TEST_NONE;
            m_pMappingName = g_NATTypeNames[NAME_UNKNOWN];
            m_pFilteringName = g_NATTypeNames[NAME_UNKNOWN];
            m_ReplyDeadline = Time(0);
            m_SentTime = Time(0);
            m_RunsTwice = false;
            SetStep(Step(&JobDetectNATType::Start, ""));
            return;
        }
        // (the results of the comparisons are not used: a removed trace of the type of the NAT)
        static_cast<void>(IsKnownType(m_pMappingName));
        static_cast<void>(IsKnownType(m_pFilteringName));
        static_cast<void>(IsDependentType(m_pMappingName));
        isSuccess = true;
        break;
    case MODE_PORT_PREDICTION:
        if (!m_RequestsRelayProbe) {
            NATProperties properties = GetRootTransport()->GetNATProperties();
            properties.m_PublicPort = m_Replies[0].m_Message.m_Port;
            properties.m_PrivatePort = m_LocalAddress.GetPortNumber();
            GetRootTransport()->SetNATProperties(properties);
        }
        isSuccess = true;
        break;
    default:
        isSuccess = false;
        break;
    }
    if (isSuccess) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_DETECTION_SUCCESS), true);
    } else {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
    }
    SetToComplete();
}

// 0x0038B62C (name is ours)
void nn::nex::JobDetectNATType::StartTests()
{
    if (m_Servers.GetSize() == 0) {
        SetStep(Step(&JobDetectNATType::Finish, ""));
        return;
    }
    m_Target = m_Servers.front();
    m_RoundTripTime = REPLY_TIME;
    m_TestState = TEST_MAPPING;
    SetStep(Step(&JobDetectNATType::SendTestRequest, ""));
}

// 0x0038B0B0 (name is ours)
void nn::nex::JobDetectNATType::SendTestRequest()
{
    m_TestMessage.m_Type = m_TestState;
    m_TestMessage.m_Unknown0xC = m_PublicAddress.GetAddress();
    m_TestMessage.ToNetworkByteOrder();
    m_Socket.SendTo(reinterpret_cast<u8*>(&m_TestMessage), sizeof(m_TestMessage), &m_Target);
    m_SentTime = Time::GetTime();
    // the reply is due after 1.25 round trip times
    u32 timeout = m_RoundTripTime + m_RoundTripTime / 4;
    if (timeout < MIN_TEST_REPLY_TIME) {
        timeout = MIN_TEST_REPLY_TIME;
    }
    m_ReplyDeadline = Time::ConvertTimeoutToDeadline(timeout);
    SetStep(Step(&JobDetectNATType::WaitForTestReply, ""));
}

// 0x0038AFFC (name is ours)
void nn::nex::JobDetectNATType::WaitForTestReply()
{
    u32 size = 0;
    if (m_Socket.RecvFrom(reinterpret_cast<u8*>(&m_Received), sizeof(m_Received), &size, &m_ReplyAddress) == Socket::RESULT_SUCCESS) {
        m_TestRetryCount = 0;
        m_Received.ToHostByteOrder();
        ProcessTestReply(m_Received);
        return;
    }
    if (m_ReplyDeadline < Time::GetTime()) {
        m_TestRetryCount++;
        ProcessTestTimeout();
        return;
    }
    SetToWaiting(TEST_WAIT_TIME);
}

// 0x00389F90 (name is ours)
void nn::nex::JobDetectNATType::ProcessTestReply(const nn::nex::NATCheckMessage& reply)
{
    if (reply.m_Type != m_TestState) {
        return;
    }
    InetAddress address;
    address.SetAddress(reply.m_Address);
    address.SetPortNumber(reply.m_Port);
    switch (m_TestState) {
    case TEST_NONE:
        break;
    case TEST_MAPPING:
        m_RoundTripTime = static_cast<u32>(Time::GetTime().m_Value) - static_cast<u32>(m_SentTime.m_Value);
        m_PublicAddress = address;
        if (m_PublicAddress.GetPortNumber() == m_LocalAddress.GetPortNumber()) {
            m_IsPortPreserved = true;
        }
        if (m_RunsTwice) {
            m_FirstRunPorts[0] = address.GetPortNumber();
        } else {
            m_SecondRunPorts[0] = address.GetPortNumber();
        }
        if (m_PublicAddress == m_LocalAddress) {
            // no NAT
            m_pMappingName = g_NATTypeNames[NAME_ENDPOINT_INDEPENDENT];
            m_pFilteringName = g_NATTypeNames[NAME_ENDPOINT_INDEPENDENT];
            SetStep(Step(&JobDetectNATType::Finish, ""));
        } else {
            SetStep(Step(&JobDetectNATType::StartFilteringTestPort, ""));
        }
        break;
    case TEST_FILTERING_PORT:
        m_pFilteringName = g_NATTypeNames[NAME_ENDPOINT_INDEPENDENT];
        SetStep(Step(&JobDetectNATType::StartMappingTestAddress, ""));
        break;
    case TEST_FILTERING_ADDRESS:
        m_pFilteringName = g_NATTypeNames[NAME_ADDRESS_DEPENDENT];
        SetStep(Step(&JobDetectNATType::StartMappingTestAddress, ""));
        break;
    case TEST_MAPPING_ADDRESS:
        if (address.GetPortNumber() == m_PublicAddress.GetPortNumber()) {
            if (m_RunsTwice) {
                m_FirstRunPorts[1] = address.GetPortNumber();
            } else {
                m_SecondRunPorts[1] = address.GetPortNumber();
            }
            m_pMappingName = g_NATTypeNames[NAME_ENDPOINT_INDEPENDENT];
            SetStep(Step(&JobDetectNATType::Finish, ""));
        } else {
            m_PublicAddress = address;
            SetStep(Step(&JobDetectNATType::StartMappingTestPort, ""));
        }
        break;
    case TEST_MAPPING_PORT:
        if (m_RunsTwice) {
            m_FirstRunPorts[1] = address.GetPortNumber();
        } else {
            m_SecondRunPorts[1] = address.GetPortNumber();
        }
        m_pMappingName = address.GetPortNumber() == m_PublicAddress.GetPortNumber() ? g_NATTypeNames[NAME_ADDRESS_DEPENDENT] : g_NATTypeNames[NAME_ADDRESS_AND_PORT_DEPENDENT];
        m_PublicAddress = address;
        SetStep(Step(&JobDetectNATType::Finish, ""));
        break;
    }
}

// 0x0038A2F8 (name is ours)
void nn::nex::JobDetectNATType::ProcessTestTimeout()
{
    switch (m_TestState) {
    case TEST_NONE:
        break;
    case TEST_MAPPING:
        // the server does not reply: the next one with the port of the target
        m_Servers.erase(m_Servers.GetFirst());
        while (!m_Servers.IsEnd(m_Servers.GetFirst()) && m_Servers.front().GetPortNumber() != m_Target.GetPortNumber()) {
            m_Servers.erase(m_Servers.GetFirst());
        }
        m_TestRetryCount = 0;
        SetStep(Step(&JobDetectNATType::StartTests, ""));
        break;
    case TEST_FILTERING_PORT:
        if (m_TestRetryCount >= MAX_TEST_RETRY_COUNT) {
            m_TestRetryCount = 0;
            SetStep(Step(&JobDetectNATType::StartFilteringTestAddress, ""));
        } else {
            SetStep(Step(&JobDetectNATType::StartFilteringTestPort, ""));
        }
        break;
    case TEST_FILTERING_ADDRESS:
        if (m_TestRetryCount >= MAX_TEST_RETRY_COUNT) {
            m_pFilteringName = g_NATTypeNames[NAME_ADDRESS_AND_PORT_DEPENDENT];
            m_TestRetryCount = 0;
            SetStep(Step(&JobDetectNATType::StartMappingTestAddress, ""));
        } else {
            SetStep(Step(&JobDetectNATType::StartFilteringTestAddress, ""));
        }
        break;
    case TEST_MAPPING_ADDRESS:
        if (m_TestRetryCount >= MAX_TEST_RETRY_COUNT) {
            m_TestRetryCount = 0;
            SetStep(Step(&JobDetectNATType::Finish, ""));
        } else {
            SetStep(Step(&JobDetectNATType::StartMappingTestAddress, ""));
        }
        break;
    case TEST_MAPPING_PORT:
        if (m_TestRetryCount >= MAX_TEST_RETRY_COUNT) {
            m_TestRetryCount = 0;
            SetStep(Step(&JobDetectNATType::Finish, ""));
        } else {
            SetStep(Step(&JobDetectNATType::StartMappingTestPort, ""));
        }
        break;
    }
}

// 0x0038B6D8 (name is ours)
void nn::nex::JobDetectNATType::StartFilteringTestPort()
{
    m_TestState = TEST_FILTERING_PORT;
    SetStep(Step(&JobDetectNATType::SendTestRequest, ""));
}

// 0x0038B724 (name is ours)
void nn::nex::JobDetectNATType::StartFilteringTestAddress()
{
    m_TestState = TEST_FILTERING_ADDRESS;
    SetStep(Step(&JobDetectNATType::SendTestRequest, ""));
}

// 0x0038B770 (name is ours)
void nn::nex::JobDetectNATType::StartMappingTestAddress()
{
    m_Servers.remove(m_Target);
    bool isFound = false;
    for (qList<InetAddress>::Node* pNode = m_Servers.GetFirst(); !m_Servers.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetAddress() != m_Target.GetAddress() && pNode->m_Value.GetPortNumber() == m_Target.GetPortNumber()) {
            m_Target = pNode->m_Value;
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        SetStep(Step(&JobDetectNATType::Finish, ""));
        return;
    }
    m_TestState = TEST_MAPPING_ADDRESS;
    SetStep(Step(&JobDetectNATType::SendTestRequest, ""));
}

// 0x0038B880 (name is ours)
void nn::nex::JobDetectNATType::StartMappingTestPort()
{
    m_Servers.remove(m_Target);
    bool isFound = false;
    for (qList<InetAddress>::Node* pNode = m_Servers.GetFirst(); !m_Servers.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetAddress() == m_Target.GetAddress() && pNode->m_Value.GetPortNumber() != m_Target.GetPortNumber()) {
            m_Target = pNode->m_Value;
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        SetStep(Step(&JobDetectNATType::Finish, ""));
        return;
    }
    m_TestState = TEST_MAPPING_PORT;
    SetStep(Step(&JobDetectNATType::SendTestRequest, ""));
}

} // namespace nex
} // namespace nn
