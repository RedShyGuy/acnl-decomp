#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATCheckMessage.h"
#include "nn/nex/nex_Socket.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// A job that sends NAT check messages to the NAT check servers of the root transport (resolving
// their URLs first) and collects the replies. armlink kept only the steps (no constructor, vtable or
// RTTI); the class name is from the fefates symbols (InitializeSocket), the layout is from the steps,
// the names of the members, the slots and the other steps are ours.
class JobNATCheckBase : public ::nn::nex::StepSequenceJob
{
public:
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

    // the requests go into m_Requests (the first step of the check)
    virtual void StartCheck() = 0; // slot 0x34
    // enough replies arrived
    virtual bool IsFinished() = 0; // slot 0x38
    virtual nn::nex::qResult GetResult() = 0; // slot 0x3C

    // the socket is closed; the call ends with the result (or the check starts again)
    void Finish(); // 0x003796CC
    // sends the requests and receives the replies until the deadline
    void ReceiveReplies(); // 0x0037986C
    // the end of the resolution of the URLs of the servers
    void WaitForURLs(); // 0x00379A8C
    void Start(); // 0x00379B6C
    bool InitializeSocket(); // 0x00379DEC | fefates:callgraph [tier C]

    InetAddress m_LocalAddress;   // 0x68
    Socket m_Socket;              // 0x88
    qList<Request> m_Requests;    // 0xCC
    NATCheckMessage m_Received;   // 0xE4
    InetAddress m_ReplyAddress;   // 0xF4
    u32 m_CallContextId;          // 0x114 (the call of the caller)
    Reply m_Replies[NATCheckMessage::TYPE_REPLY_NUM]; // 0x118 (by type, from TYPE_REPLY_MIN)
    u32 m_Unknown0x154;           // 0x154
    Time m_Deadline;              // 0x158 (of the replies)
    u32 m_RetryCount;             // 0x160
    u32 m_Unknown0x164;           // 0x164
    CallContext m_ResolveContext; // 0x168 (of the resolution of the URLs)
};
ASSERT_OFFSET(JobNATCheckBase, m_Socket, 0x88);
ASSERT_OFFSET(JobNATCheckBase, m_Requests, 0xCC);
ASSERT_OFFSET(JobNATCheckBase, m_CallContextId, 0x114);
ASSERT_OFFSET(JobNATCheckBase, m_Replies, 0x118);
ASSERT_OFFSET(JobNATCheckBase, m_Deadline, 0x158);
ASSERT_OFFSET(JobNATCheckBase, m_ResolveContext, 0x168);
ASSERT_SIZE(JobNATCheckBase, 0x1C8);

// (out of line in the original; nex_JobNATCheckBase.cpp)
template <>
DECOMP_NOINLINE void qList<JobNATCheckBase::Request>::push_back(const JobNATCheckBase::Request& value);

// The NAT check of the public address: the replies of type 101 of one server (the second one if the
// NAT maps per destination). The name is ours.
class JobCheckPublicAddress : public ::nn::nex::JobNATCheckBase
{
public:
    virtual void StartCheck(); // 0x00393374 slot 0x34 (name is ours)
    void SendRequests(); // 0x00393458 (name is ours)
};

// The NAT check of the mapping and the filtering: the replies of the types 101 to 103. The name is
// ours.
class JobCheckNATProperties : public ::nn::nex::JobNATCheckBase
{
public:
    virtual void StartCheck(); // 0x003986C4 slot 0x34 (name is ours)
    void SendRequests(); // 0x003987A8 (name is ours)

    Time m_StartTime; // 0x1C8 (of the requests)
};
ASSERT_OFFSET(JobCheckNATProperties, m_StartTime, 0x1C8);
} // namespace nex
} // namespace nn
