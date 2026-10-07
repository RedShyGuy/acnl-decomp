#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
class Socket;

// RTTI N2nn3pia4inet16SocketStreamBaseE @ 0x008CF8A8
// vtable 0x00900060 (vptr 0x00900068), offset_to_top 0, 2 entries
//
// Base of the socket streams: a buffer of the largest MTU and the socket of the stream. The layout
// is from the constructor; the member names and the names marked so are ours.
class SocketStreamBase : public ::nn::pia::common::RootObject
{
public:
    static const unsigned int BUFFER_SIZE = 0x554;

    SocketStreamBase(); // 0x003E7FAC | fefates:bytes [tier B]
    virtual ~SocketStreamBase(); // 0x003E8000 slot 0x00 | fefates:bytes
    // 0x003E7FE4 slot 0x04 (deleting dtor)

    nn::Result ImportSocket(nn::pia::inet::Socket* pSocket); // 0x003E7F84 | fefates:callgraph [tier C]
    nn::Result Startup(); // 0x003E7F9C | fefates:callgraph [tier C]
    // (name is ours)
    void Cleanup(); // 0x003E7F90

    u8 m_Buffer[BUFFER_SIZE];  // 0x004
    bool m_IsStarted;          // 0x558
    Socket* m_pSocket;         // 0x55C
};
ASSERT_OFFSET(SocketStreamBase, m_IsStarted, 0x558);
ASSERT_SIZE(SocketStreamBase, 0x560);
} // namespace inet
} // namespace pia
} // namespace nn
