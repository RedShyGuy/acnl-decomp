#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nex/nex_NATCheckMessage.h"
#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatDetectionJob.h"
#include "nn/pia/inet/inet_Socket.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
} // namespace common
namespace inet {
// RTTI N2nn3pia4inet11NatDetecterE @ 0x008CF818
// vtable 0x008FFE6C (vptr 0x008FFE74), offset_to_top 0, 12 entries
//
// The base of the NAT checks of NatTraverser: it sends nex::NATCheckMessages from a socket of its
// own to the NAT check servers and keeps the replies; the derived detecters decide what to send
// and what the replies mean. NatDetectionJob runs the check. The member names are ours.
class NatDetecter : public ::nn::pia::common::RootObject
{
public:
    // the local ports of the socket are chosen from this range
    static u16 s_PortMin; // 0x0097FA0A
    static u16 s_PortMax; // 0x0097FA0C

    // a message for the send queue: the server and the message (the name is from the RTTI of the
    // list, the layout from AddSendMessage)
    struct SendNatCheckMessage
    {
        SendNatCheckMessage() {}
        SendNatCheckMessage(common::InetAddress address, u32 type) : m_Address(address)
        {
            m_Message.m_Type = type;
        }

        common::InetAddress m_Address;  // 0x00
        nex::NATCheckMessage m_Message; // 0x08
    };

    // RTTI N2nn3pia4inet11NatDetecter23SendNatCheckMessageListE @ 0x008CF800
    // vtable 0x008FFE5C (vptr 0x008FFE64), offset_to_top 0, 2 entries
    class SendNatCheckMessageList : public ::nn::pia::common::FixedObjList<SendNatCheckMessage, 20u>
    {
    public:
        virtual ~SendNatCheckMessageList(); // 0x003E35E0 slot 0x00
        // 0x003E35DC slot 0x04 (deleting dtor)
    };

    // the last reply of one type and how many of them arrived (name is ours)
    struct ReceivedMessage
    {
        ReceivedMessage() : m_Count(0) {} // 0x003E35C0 (the element constructor of the array)

        u32 m_Count;                    // 0x00
        nex::NATCheckMessage m_Message; // 0x04
    };

    NatDetecter(); // 0x003E38D0 | fefates:bytes [tier B]
    virtual ~NatDetecter(); // 0x003E3A08 slot 0x00 | fefates:bytes
    // 0x003E39C0 slot 0x04 (deleting dtor)
    // the local address the socket binds to (port 0: a random one)
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress); // 0x003E375C slot 0x08 | fefates:callseq
    virtual void Cleanup(); // 0x003E3698 slot 0x0C | fefates:bytes
    // the check runs once more (armlink placed it in front of ListBase::Init)
    virtual void Retry(); // 0x004296DC slot 0x10 | fefates:bytes
    virtual void StartSendingMessage() = 0; // slot 0x14
    // true when all replies the check needs arrived
    virtual bool CheckAllMessage() = 0; // slot 0x18
    // evaluates the replies; false if the check failed
    virtual bool HandleResult() = 0; // slot 0x1C
    // true if the check should run once more
    virtual bool CheckRetry(); // 0x003E2F28 slot 0x20 | slot vf_0x20 of nn::pia::inet::NatDetecter
    // the time the check waits for the replies in milliseconds
    virtual u32 GetDetectionTimeout() const = 0; // slot 0x24
    // (armlink placed both in front of the functions they call last)
    virtual void StartDetectionJob(); // 0x004288B8 slot 0x28 | slot vf_0x28 of nn::pia::inet::NatDetecter
    virtual void CancelDetectionJob(); // 0x00427438 slot 0x2C | fefates:bytes

    nn::Result OpenSocket(); // 0x003E2F30 | fefates:bytes [tier B]
    nn::Result CloseSocket(); // 0x003E3078 | fefates:bytes [tier B]
    // a new socket on another random port
    nn::Result RetryBind(); // 0x003E37F0 | fefates:bytes [tier B]
    // queues num copies of the message
    void AddSendMessage(const nn::pia::inet::NatDetecter::SendNatCheckMessage& message, unsigned short num); // 0x003E3148 | fefates:bytes [tier B]
    // sends the queue; false if a send failed
    bool SendAllQueuedMessage(); // 0x003E3468 | fefates:bytes [tier B]
    // reads one reply from the socket; false if none arrived (name is ours)
    bool ReceiveMessage(); // 0x003E3210
    void InitializeReceiveMessage(); // 0x003E35E4 | fefates:bytes [tier B]
    ReceivedMessage* GetReceiveMessage(unsigned int index); // 0x003E3458 | fefates:bytes [tier B]
    void TraceReceivedMessageArray(unsigned long long flag); // 0x003E3618 | fefates:bytes [tier B]
    // the port the server of the first reply saw
    u16 GetPerceivedPort(); // 0x003E32F4 | fefates:bytes [tier B]
    // the address the next mapping of the NAT probably gets: the one of the first reply with the
    // port the NAT increments to (name is ours)
    void GetPredictedAddress(nn::pia::common::InetAddress* pAddress); // 0x003E30D0
    // 5 messages without a type to the primary server, to open the NAT
    void sendDummyMessage(); // 0x003E3300 | fefates:bytes [tier B]
    // a random port of the range other than port (0 if 100 tries failed)
    static u16 GetDifferentPortNumber(unsigned short port); // 0x003E3568 | fefates:bytes [tier B]
    static const common::InetAddress* GetPrimaryServerPrimaryPortAddress(); // 0x003E362C | fefates:bytes [tier B]
    static const common::InetAddress* GetPrimaryServerSecondaryPortAddress(); // 0x003E365C | fefates:bytes [tier B]

    Socket m_Socket;                                                 // 0x004
    u32 m_RetryCount;                                                // 0x014
    common::InetAddress m_LocalAddress;                              // 0x018
    common::InetAddress m_ReceivedFromAddress;                       // 0x020
    nex::NATCheckMessage m_SendBuffer;                               // 0x028
    nex::NATCheckMessage m_ReceiveBuffer;                            // 0x038
    SendNatCheckMessageList m_SendList;                              // 0x048
    ReceivedMessage m_ReceivedMessages[nex::NATCheckMessage::TYPE_REPLY_NUM]; // 0x2F8
    // the time from the first send to the first reply in milliseconds (NatDetectionJob)
    u32 m_Rtt;                                                       // 0x334
    u32 m_Unknown0x338;                                              // 0x338 (not used here)
    NatDetectionJob m_DetectionJob;                                  // 0x340
};
ASSERT_OFFSET(NatDetecter, m_SendList, 0x48);
ASSERT_OFFSET(NatDetecter, m_ReceivedMessages, 0x2F8);
ASSERT_OFFSET(NatDetecter, m_Rtt, 0x334);
ASSERT_OFFSET(NatDetecter, m_DetectionJob, 0x340);
ASSERT_SIZE(NatDetecter, 0x3B8);
} // namespace inet
} // namespace pia
} // namespace nn
