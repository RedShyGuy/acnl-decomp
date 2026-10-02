#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class RoutingTable
{
public:
    RoutingTable(); // TODO: default ctor added so derived stubs compile - may not exist
    void Add(const nn::nex::InetAddress&, const nn::nex::InetAddress&, bool); // 0x003604AC | fefates:bytes [tier B]
    void Remove(const nn::nex::InetAddress&); // 0x003606EC | fefates:bytes [tier B]
    RoutingTable(unsigned int); // 0x00360828 | fefates:bytes [tier B]
    void Find(const nn::nex::InetAddress&, nn::nex::InetAddress&) const; // 0x0072A538 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
