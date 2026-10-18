#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class PRUDPEndPoint;

// The virtual port of a PRUDP end point at an address (the "sid" of its URL). The member name is
// ours.
struct VirtualPort
{
    VirtualPort() : m_Value(0) {}
    explicit VirtualPort(u8 value) : m_Value(value) {}
    VirtualPort(const VirtualPort& other) : m_Value(other.m_Value) {}

    u8 m_Value;
};

// The key of an end point: the address and the virtual port. The member names are ours.
struct EndPointUniqueID
{
    // (inline)
    EndPointUniqueID(const InetAddress& address, const VirtualPort& port) : m_Port(port.m_Value)
    {
        m_Address.m_SockAddr = address.m_SockAddr;
    }

    // (inline) by address, port and virtual port
    bool operator<(const EndPointUniqueID& other) const
    {
        s64 a = (static_cast<s64>(static_cast<s32>(m_Address.m_SockAddr.m_Address)) << 32) | m_Address.m_SockAddr.m_Port;
        s64 b = (static_cast<s64>(static_cast<s32>(other.m_Address.m_SockAddr.m_Address)) << 32) | other.m_Address.m_SockAddr.m_Port;
        if (a != b) {
            return a < b;
        }
        return m_Port < other.m_Port;
    }

    InetAddress m_Address; // 0x00
    u8 m_Port;             // 0x20
};
ASSERT_SIZE(EndPointUniqueID, 0x24);

// The end points of a PRUDPStream by address and virtual port. The name is from the reference
// symbols.
class PRUDPEndPointMap : public std::map<EndPointUniqueID, PRUDPEndPoint*>
{
public:
    // the end point at the address and virtual port, null if there is none
    nn::nex::PRUDPEndPoint* Locate(const nn::nex::InetAddress& address, nn::nex::VirtualPort port); // 0x003833E4 | fefates:bytes [tier B]
};
ASSERT_SIZE(PRUDPEndPointMap, 0x1C);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE map<nn::nex::EndPointUniqueID, nn::nex::PRUDPEndPoint*>::iterator map<nn::nex::EndPointUniqueID, nn::nex::PRUDPEndPoint*>::find(const nn::nex::EndPointUniqueID& key);
} // namespace std
