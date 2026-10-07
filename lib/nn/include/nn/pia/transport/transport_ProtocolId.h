#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
// The id of a protocol: its type in the high and its port in the low half (Protocol::SetPort; in the
// messages big endian). The type name is from the signatures; the members are ours.
// The protocol types. The names are from PacketAnalysisData::Print (BANDWIDTH_CHECKER from its
// class, Print has no name for it); the enumerator names are ours.
enum ProtocolType : u16
{
    PROTOCOL_TYPE_RELAY = 0x0080,
    PROTOCOL_TYPE_KEEP_ALIVE = 0x00C0,
    PROTOCOL_TYPE_STATION = 0x0100,
    PROTOCOL_TYPE_MESH = 0x0200,
    PROTOCOL_TYPE_SYNC_CLOCK = 0x0210,
    PROTOCOL_TYPE_NAT = 0x0400,
    PROTOCOL_TYPE_GATEWAY = 0x0410,
    PROTOCOL_TYPE_UNKNOWN_0420 = 0x0420, // only in ProtocolManager::CreateProtocolImpl
    PROTOCOL_TYPE_BANDWIDTH_CHECKER = 0x0500,
    PROTOCOL_TYPE_RTT = 0x0600,
    PROTOCOL_TYPE_SYNC_OLD = 0x1800,
    PROTOCOL_TYPE_SYNC = 0x1810,
    PROTOCOL_TYPE_UNRELIABLE = 0x2000,
    PROTOCOL_TYPE_ROUNDROBIN_UNRELIABLE = 0x2100,
    PROTOCOL_TYPE_CLONE = 0x2400,
    PROTOCOL_TYPE_VOICE = 0x2800,
    PROTOCOL_TYPE_RELIABLE = 0x3000,
    PROTOCOL_TYPE_RELIABLE_BROADCAST = 0x7000,
    PROTOCOL_TYPE_SESSION = 0x7200,
    PROTOCOL_TYPE_FEEDBACK = 0x8000,
    PROTOCOL_TYPE_RELAY_SERVICE = 0x8200,
};

class ProtocolId
{
public:
    ProtocolId() {}
    ProtocolId(u16 type, u16 port) : m_Id((static_cast<u32>(type) << 16) | port) {}

    u16 GetType() const { return m_Id >> 16; }
    u16 GetPort() const { return m_Id; }
    void SetType(u16 type) { m_Id = (m_Id & 0xFFFF) | (static_cast<u32>(type) << 16); }
    void SetPort(u16 port) { m_Id = (m_Id & 0xFFFF0000) | port; }
    bool operator==(const ProtocolId& rhs) const { return m_Id == rhs.m_Id; }
    bool operator!=(const ProtocolId& rhs) const { return m_Id != rhs.m_Id; }

    // the id of no protocol (0; the static initializer 0x0079DBEC sets it)
    static const ProtocolId INVALID;

    u32 m_Id; // 0x0
};
ASSERT_SIZE(ProtocolId, 0x4);
} // namespace transport
} // namespace pia
} // namespace nn
