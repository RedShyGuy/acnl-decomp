#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045F9D0 | slot vf_0x1C of nn::pia::transport::Protocol
nn::Result nn::pia::transport::Protocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
    return nn::Result();
}

// 0x0045F9D8 | slot vf_0x14 of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::Cleanup()
{
    // empty (in the original too)
}

// 0x0045F9DC | fefates:bytes [tier B]
void nn::pia::transport::Protocol::SetPort(unsigned short port)
{
    m_ProtocolId = ProtocolId(GetProtocolType(), port);
}

// 0x0045FA18 | slot vf_0x10 of nn::pia::transport::Protocol
nn::Result nn::pia::transport::Protocol::Startup(nn::pia::StationIndex)
{
    return nn::Result();
}

// 0x0045FA20 | slot vf_0x18 of nn::pia::transport::Protocol
nn::Result nn::pia::transport::Protocol::Dispatch()
{
    return nn::Result();
}

// 0x0045FA28 | fefates:bytes [tier B]
nn::pia::transport::Protocol::Protocol() : m_ProtocolId(ProtocolId::INVALID)
{
}

// 0x0045FA58 | fefates:callgraph [tier C]
// 0x0045FA50 (deleting dtor)
nn::pia::transport::Protocol::~Protocol()
{
    // nothing to do: the members are destroyed by the compiler
}

// 0x00736E78 | slot vf_0x20 of nn::pia::transport::Protocol
bool nn::pia::transport::Protocol::IsEnableProtocolFiltering() const
{
    return true;
}

// 0x00736E80 (name after StepSequenceJob::Trace)
void nn::pia::transport::Protocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
