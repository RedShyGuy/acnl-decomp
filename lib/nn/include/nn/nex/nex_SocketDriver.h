#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The sockets of the platform for nex (BerkeleySocketDriver implements it). The type names are from
// the reference symbols; the names of the members, of the values and of the slots without a reference
// symbol are ours.
// RTTI N2nn3nex12SocketDriverE @ 0x008CE1B8
class SocketDriver : public ::nn::nex::RootObject
{
public:
    class Socket;

    // an address in host byte order
    struct InetAddress
    {
        u32 m_Address; // 0x00
        u16 m_Port;    // 0x04
    };
    // a socket to wait for (m_Events, m_ReturnedEvents: EVENT_*)
    struct PollInfo
    {
        Socket* m_pSocket;  // 0x00
        u8 m_Events;        // 0x04
        u8 m_ReturnedEvents; // 0x05
    };
    enum _TrafficType
    {
        TRAFFIC_TYPE_DATAGRAM = 0,
        TRAFFIC_TYPE_STREAM = 1,
    };
    enum _SocketFlag
    {
        SOCKET_FLAG_NONE = 0,
        SOCKET_FLAG_DONT_WAIT = 1,
    };
    // the results of the calls of a socket
    enum _SocketResult
    {
        SOCKET_RESULT_SUCCESS = 0,
        SOCKET_RESULT_ERROR = 1,
        SOCKET_RESULT_WOULD_BLOCK = 2,
        SOCKET_RESULT_IN_PROGRESS = 3,
        SOCKET_RESULT_CLOSED = 4,
    };
    // the events of Poll
    static const u8 EVENT_READ = 1;
    static const u8 EVENT_WRITE = 2;

    // (inline)
    SocketDriver() {}
    virtual ~SocketDriver() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    virtual Socket* Create() = 0; // slot 0x08
    virtual void Delete(Socket* pSocket) = 0; // slot 0x0C
    // false: no socket is ready (or an error)
    virtual bool Poll(PollInfo* pInfos, unsigned int count, unsigned int timeout) = 0; // slot 0x10

    // the driver of the sockets (the one of the transport adapter, else the default one; the names
    // are ours)
    static SocketDriver* s_pInstance;
    static SocketDriver* s_pDefaultInstance;
};
ASSERT_SIZE(SocketDriver::InetAddress, 0x8);
ASSERT_SIZE(SocketDriver::PollInfo, 0x8);
} // namespace nex
} // namespace nn
