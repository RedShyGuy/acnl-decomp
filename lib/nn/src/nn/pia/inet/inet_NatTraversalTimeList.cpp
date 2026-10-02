#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/inet/inet_NatTraversalTimeList.h"

namespace nn {
namespace pia {
namespace inet {
// ctor address unknown
nn::pia::inet::NatTraversalTimeList::NatTraversalTimeList()
{
}

// 0x004000D4 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatTraversalTimeList
void nn::pia::inet::NatTraversalTimeList::vf_0x00()
{
}

// 0x00400064 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatTraversalTimeList
void nn::pia::inet::NatTraversalTimeList::vf_0x04()
{
}

// 0x003FFE2C | fefates:bytes-fuzzy [tier B]
void nn::pia::inet::NatTraversalTimeList::Add(const nn::pia::inet::NatTraversalTime&)
{
}

// 0x003FFF40 | fefates:bytes [tier B]
void nn::pia::inet::NatTraversalTimeList::Find(const nn::pia::transport::StationLocation&)
{
}

// 0x003FFFB0 | fefates:bytes [tier B]
void nn::pia::inet::NatTraversalTimeList::Remove(const nn::pia::transport::StationLocation&)
{
}

} // namespace inet
} // namespace pia
} // namespace nn
