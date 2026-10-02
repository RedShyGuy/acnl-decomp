#include "nn/pia/inet/inet_NatProbe.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/inet/inet_NatProbeList.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E5070 slot 0x00 | fefates:callgraph
void nn::pia::inet::NatProbeList::vf_0x00()
{
}

// 0x003E4FB8 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeList
void nn::pia::inet::NatProbeList::vf_0x04()
{
}

// 0x0072EFEC slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbeList
void nn::pia::inet::NatProbeList::vf_0x08()
{
}

// 0x003E4554 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::ProcessProbe(const nn::pia::transport::StationLocation&, unsigned char)
{
}

// 0x003E4698 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::ProcessProbeReply(const nn::pia::transport::StationLocation&, nn::pia::common::Time, unsigned char)
{
}

// 0x003E4794 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::FindByConnectionId(unsigned int)
{
}

// 0x003E47C4 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeNotReceivedProbes(unsigned int)
{
}

// 0x003E4868 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::RemoveProbesByConnectionId(unsigned int)
{
}

// 0x003E4900 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::FindByAddressAndConnectionId(const nn::pia::transport::StationLocation&)
{
}

// 0x003E4958 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::FindByAddressPortAndConnectionId(const nn::pia::transport::StationLocation&)
{
}

// 0x003E49CC | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::RemoveSameAddressPortAndConnectionIdProbes(const nn::pia::transport::StationLocation&)
{
}

// 0x003E4AA0 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeDifferentConnectionIdAndSameAddressPortProbes(const nn::pia::transport::StationLocation&)
{
}

// 0x003E4B68 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeDifferentPortAndSameAddressAndConnectionIdProbes(const nn::pia::transport::StationLocation&)
{
}

// 0x003E4C14 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::AddProbe(const nn::pia::inet::NatProbe&)
{
}

// 0x003E4E80 | fefates:bytes [tier B]
nn::pia::inet::NatProbeList::NatProbeList()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
