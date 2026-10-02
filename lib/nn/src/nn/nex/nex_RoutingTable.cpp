#include "nn/nex/nex_RoutingTable.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::RoutingTable::RoutingTable()
{
}

// 0x003604AC | fefates:bytes [tier B]
void nn::nex::RoutingTable::Add(const nn::nex::InetAddress&, const nn::nex::InetAddress&, bool)
{
}

// 0x003606EC | fefates:bytes [tier B]
void nn::nex::RoutingTable::Remove(const nn::nex::InetAddress&)
{
}

// 0x00360828 | fefates:bytes [tier B]
nn::nex::RoutingTable::RoutingTable(unsigned int)
{
}

// 0x0072A538 | fefates:bytes [tier B]
void nn::nex::RoutingTable::Find(const nn::nex::InetAddress&, nn::nex::InetAddress&) const
{
}

} // namespace nex
} // namespace nn
