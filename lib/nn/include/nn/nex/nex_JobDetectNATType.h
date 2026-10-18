#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATCheckMessage.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_Socket.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class NATRelayInterface;

// Detects the type of the NAT with the NAT check servers of the root transport: the mapping and the
// filtering from the replies of several servers (MODE_PROPERTIES), by a sequence of tests of the
// mapping and the filtering in the style of RFC 4787 (MODE_TESTS, with the names of the types), or
// the next public port for a relay probe (MODE_PORT_PREDICTION). armlink kept only the steps (no
// constructor, vtable or RTTI); the class name and all member and step names are ours.
class JobDetectNATType : public ::nn::nex::StepSequenceJob
{
public:
    enum Mode : u8
    {
        MODE_PROPERTIES = 1,
        MODE_TESTS = 2,
        MODE_PORT_PREDICTION = 3,
    };

    // a message for a server
    struct Request
    {
        Request(InetAddress address, u32 type) : m_Address(address)
        {
            m_Message.m_Type = type;
            m_Message.ToNetworkByteOrder();
        }

        InetAddress m_Address;     // 0x00
        NATCheckMessage m_Message; // 0x20 (network byte order)
    };
    // the last reply of a type and the number of the replies
    struct Reply
    {
        Reply() : m_Count(0) {}

        NATCheckMessage m_Message; // 0x00
        u32 m_Count;               // 0x10
    };

    // the servers and the socket
    void Start(); // 0x00389C54
    // MODE_PROPERTIES / MODE_PORT_PREDICTION: the requests to the servers
    void SendRequests(); // 0x0038B16C
    void ReceiveReplies(); // 0x0038ACB0
    // the port of the next mapping goes to the relay interface
    void PredictPort(); // 0x0038A5DC
    // the result goes to the call (MODE_TESTS may run a second time)
    void Finish(); // 0x0038A95C
    // MODE_PROPERTIES: the NAT properties of the root transport from the replies
    DECOMP_NOINLINE bool UpdateNATProperties(); // 0x0038A7AC

    // MODE_TESTS: the test of the state m_TestState with m_Target
    void StartTests(); // 0x0038B62C
    void SendTestRequest(); // 0x0038B0B0
    void WaitForTestReply(); // 0x0038AFFC
    DECOMP_NOINLINE void ProcessTestReply(const nn::nex::NATCheckMessage& reply); // 0x00389F90
    DECOMP_NOINLINE void ProcessTestTimeout(); // 0x0038A2F8
    // the filtering tests (the reply from another port / another address)
    void StartFilteringTestPort(); // 0x0038B6D8
    void StartFilteringTestAddress(); // 0x0038B724
    // the mapping tests: another server with the same port / the same server with another port
    void StartMappingTestAddress(); // 0x0038B770
    void StartMappingTestPort(); // 0x0038B880

    u8 m_Unknown0x68[0xA8];             // 0x68
    qList<InetAddress> m_Servers;       // 0x110 (the NAT check servers)
    InetAddress m_LocalAddress;         // 0x128 (of the socket)
    InetAddress m_PublicAddress;        // 0x148 (the mapped address of the tests)
    InetAddress m_ReplyAddress;         // 0x168
    Mode m_Mode;                        // 0x188
    NATCheckMessage m_Received;         // 0x18C
    Socket m_Socket;                    // 0x19C
    u32 m_CallContextId;                // 0x1E0 (the call of the caller)
    NATRelayInterface* m_pRelayInterface; // 0x1E4
    StationURL m_TargetURL;             // 0x1E8 (of the relay probe)
    StationURL m_PredictedURL;          // 0x250 (this station with the predicted port)
    u32 m_TestRetryCount;               // 0x2B8
    u32 m_RoundTripTime;                // 0x2BC (ms, of the last test reply)
    u32 m_TestState;                    // 0x2C0 (the test, 1 to 5; the type of its message)
    const wchar_t* m_pMappingName;      // 0x2C4 (g_NATTypeNames)
    const wchar_t* m_pFilteringName;    // 0x2C8
    u32 m_Unknown0x2CC;                 // 0x2CC
    Time m_ReplyDeadline;               // 0x2D0
    Time m_SentTime;                    // 0x2D8 (of the test request)
    InetAddress m_Target;               // 0x2E0 (the server of the test)
    NATCheckMessage m_TestMessage;      // 0x300
    bool m_IsPortPreserved;             // 0x310 (the public port of the tests is the local one)
    bool m_RunsTwice;                   // 0x311 (MODE_TESTS: the first of two runs)
    u16 m_FirstRunPorts[2];             // 0x312 (the public ports of the first run)
    u16 m_SecondRunPorts[2];            // 0x316 (of the second run)
    u8 m_Unknown0x31A[0x2];             // 0x31A
    u32 m_RetryCount;                   // 0x31C (of the requests)
    bool m_HasReplies;                  // 0x320 (enough replies arrived)
    qList<Request> m_Requests;          // 0x324
    NATProperties m_Properties;         // 0x33C
    Reply m_Replies[NATCheckMessage::TYPE_REPLY_NUM]; // 0x360 (by type, from TYPE_REPLY_MIN)
    bool m_RequestsRelayProbe;          // 0x39C (MODE_PORT_PREDICTION)
};
ASSERT_OFFSET(JobDetectNATType, m_Servers, 0x110);
ASSERT_OFFSET(JobDetectNATType, m_Mode, 0x188);
ASSERT_OFFSET(JobDetectNATType, m_Socket, 0x19C);
ASSERT_OFFSET(JobDetectNATType, m_CallContextId, 0x1E0);
ASSERT_OFFSET(JobDetectNATType, m_PredictedURL, 0x250);
ASSERT_OFFSET(JobDetectNATType, m_TestRetryCount, 0x2B8);
ASSERT_OFFSET(JobDetectNATType, m_ReplyDeadline, 0x2D0);
ASSERT_OFFSET(JobDetectNATType, m_Target, 0x2E0);
ASSERT_OFFSET(JobDetectNATType, m_TestMessage, 0x300);
ASSERT_OFFSET(JobDetectNATType, m_RetryCount, 0x31C);
ASSERT_OFFSET(JobDetectNATType, m_Requests, 0x324);
ASSERT_OFFSET(JobDetectNATType, m_Properties, 0x33C);
ASSERT_OFFSET(JobDetectNATType, m_Replies, 0x360);
ASSERT_OFFSET(JobDetectNATType, m_RequestsRelayProbe, 0x39C);
ASSERT_SIZE(JobDetectNATType, 0x3A0);

// the names of the types of the mapping and of the filtering
extern const wchar_t* g_NATTypeNames[4];

// (out of line in the original; nex_JobDetectNATType.cpp)
template <>
DECOMP_NOINLINE void qList<JobDetectNATType::Request>::push_back(const JobDetectNATType::Request& value);
template <>
DECOMP_NOINLINE void qList<InetAddress>::remove(const InetAddress& value);
} // namespace nex
} // namespace nn
