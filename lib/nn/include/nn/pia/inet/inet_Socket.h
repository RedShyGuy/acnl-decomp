#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
struct SockAddrIn;

// A UDP socket of the socket service (a RootObject: NexFacade creates it with the pia heap). The
// layout is from the constructor; the member names are ours.
class Socket : public ::nn::pia::common::RootObject
{
public:
    // the most addresses SendToMulti sends to at once
    static const int SEND_ADDRESS_NUM_MAX = 12;

    Socket(); // 0x00412BCC | fefates:bytes [tier B]
    ~Socket(); // 0x00412BF4 | fefates:bytes [tier B]

    // a datagram socket (name is ours)
    nn::Result Open(); // 0x00412698
    nn::Result Open(int type, int protocol); // 0x004126A4 | fefates:bytes [tier B]
    nn::Result Close(); // 0x004127A0 | fefates:bytes [tier B]
    nn::Result Bind(const nn::pia::common::InetAddress& address); // 0x00412524 | fefates:bytes [tier B]
    nn::Result Bind(unsigned short port); // 0x00412650 | fefates:bytes [tier B]
    nn::Result SetTtl(unsigned char ttl); // 0x0041295C | fefates:bytes [tier B]
    nn::Result SendTo(const void* pData, int size, const nn::pia::common::InetAddress& address, int* pSentSize); // 0x00412898 | fefates:bytes [tier B]
    nn::Result SendToMulti(const void* pData, int size, const nn::pia::inet::SockAddrIn* pAddresses, int addressNum, int* pSentSize); // 0x0041243C | fefates:bytes [tier B]
    // without blocking; RESULT_NO_DATA if nothing arrived
    nn::Result RecvFrom(unsigned char* pBuffer, int size, nn::pia::common::InetAddress* pAddress, unsigned char* pTtl, int* pReceivedSize); // 0x00412A6C | fefates:callgraph [tier C]

    s32 m_Descriptor;                    // 0x0 (-1: closed)
    s32 m_OpenNum;                       // 0x4, Open adds one, Close subtracts one
    common::InetAddress m_BoundAddress;  // 0x8
};
ASSERT_SIZE(Socket, 0x10);
} // namespace inet
} // namespace pia
} // namespace nn
