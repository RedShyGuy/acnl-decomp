#include "nn/pia/inet/inet_NatProbe.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/session/session_Mesh.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const s32 UPDATE_INTERVAL_MSEC = 90000;

// (inline; name is ours)
inline void CountPublicAddressProbe()
{
    if (session::Mesh::s_pInstance == nullptr || !session::Mesh::s_pInstance->IsMonitoringDataSenderFlagSet()) {
        u16 count = common::g_SessionBeginMonitoringContent.m_Unknown0x49E;
        common::g_SessionBeginMonitoringContent.m_Unknown0x49E = count == 0xFFFF ? 1 : count + 1;
    }
}
} // namespace

// 0x00412C90 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::SprayTargetPort()
{
    u16 range = m_PortMax - m_PortMin + 1;
    if (range == 1) {
        return;
    }
    m_Location.m_StationAddress.m_InetAddress.m_Port = static_cast<u16>(m_SprayCount) % range + m_PortMin;
}

// 0x00412CCC | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::UpdateTargetPort(const nn::pia::transport::StationLocation& location)
{
    if (m_UpdateTime.m_Tick == 0 || m_Location.m_ProbeRequestInitiation == 0) {
        m_Location.m_StationAddress.m_InetAddress.m_Port = location.m_StationAddress.m_InetAddress.m_Port;
    }
}

// 0x00412CF0 | fefates:bytes [tier B]
u16 nn::pia::inet::NatProbe::GetPortSprayCount()
{
    return m_Location.m_StationAddress.m_InetAddress.m_Port - m_PortMin;
}

// 0x00412D04
bool nn::pia::inet::NatProbe::UpdateTargetAddress(const nn::pia::transport::StationLocation& location)
{
    if (m_Location.m_StationAddress.m_InetAddress.IsPrivate() && !location.m_StationAddress.m_InetAddress.IsPrivate()) {
        // the public address of a station known by its private one
        CountPublicAddressProbe();
        if (m_Rtt != RTT_NONE) {
            return true;
        }
    } else if (!m_Location.m_StationAddress.m_InetAddress.IsPrivate() && location.m_StationAddress.m_InetAddress.IsPrivate()) {
        CountPublicAddressProbe();
    } else if (m_UpdateTime.m_Tick != 0 && !(m_Location.m_StationAddress == location.m_StationAddress)) {
        return false;
    }
    m_Location.m_StationAddress.SetInetAddress(location.m_StationAddress.m_InetAddress);
    m_PortMin = location.m_StationAddress.m_InetAddress.m_Port;
    m_PortMax = location.m_StationAddress.m_InetAddress.m_Port;
    return true;
}

// 0x00412E38 | fefates:bytes [tier B]
void nn::pia::inet::NatProbe::UpdateRtt(const nn::pia::common::Time& now, const nn::pia::common::Time& sendTime)
{
    m_Rtt = (now - sendTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
}

// 0x00412E70 | fefates:bytes [tier B]
nn::pia::inet::NatProbe::NatProbe(const nn::pia::transport::StationLocation& location, const nn::pia::common::Time& time, const nn::pia::common::TimeSpan& timeout, unsigned char kind, unsigned char sprayCountMax, unsigned char portRange)
    : m_Location(location), m_UpdateTime(time), m_Deadline(), m_Unknown0x40(0), m_Unknown0x41(0), m_Unknown0x42(0), m_Rtt(RTT_NONE), m_Kind(kind),
      m_SprayCount(0), m_SprayCountMax(sprayCountMax),
      m_UpdateInterval(common::TimeSpan::GetTicksPerMSec().GetTick() * UPDATE_INTERVAL_MSEC)
{
    common::Time now;
    now.SetNow();
    m_Deadline = now + timeout;
    u16 port = location.m_StationAddress.m_InetAddress.m_Port;
    m_PortMin = port;
    m_PortMax = port + portRange;
    if (m_PortMax < port) {
        m_PortMax = 0xFFFF;
    }
}

// 0x00412F98
// 0x00412F78 (deleting dtor)
nn::pia::inet::NatProbe::~NatProbe()
{
    // empty (in the original too)
}

// 0x0072FA74 | fefates:bytes [tier B]
bool nn::pia::inet::NatProbe::UpdateIsNeeded(nn::pia::common::Time now) const
{
    if (m_Rtt == RTT_NONE) {
        return m_SprayCount < m_SprayCountMax;
    }
    return m_UpdateInterval.m_Tick < (now - m_UpdateTime).GetTick();
}

// 0x0072FACC slot 0x08
void nn::pia::inet::NatProbe::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
