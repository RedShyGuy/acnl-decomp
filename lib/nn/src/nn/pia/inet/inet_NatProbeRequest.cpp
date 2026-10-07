#include "nn/pia/inet/inet_NatProbeRequest.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E7E9C | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequest::NatProbeRequest(const nn::pia::transport::StationLocation& location)
    : m_Location(location), m_ConnectionInfo(), m_IsInverse(false), m_Unknown0x88(), m_pCallContext(nullptr)
{
}

// 0x003E7EDC | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequest::NatProbeRequest(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, bool isInverse, nn::pia::common::CallContext* pCallContext)
    : m_Location(location), m_ConnectionInfo(info), m_IsInverse(isInverse), m_Unknown0x88(), m_pCallContext(pCallContext)
{
}

// 0x003E7F60
// 0x003E7F38 (deleting dtor)
nn::pia::inet::NatProbeRequest::~NatProbeRequest()
{
    // empty (in the original too)
}

// 0x0072F100 slot 0x08
void nn::pia::inet::NatProbeRequest::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
