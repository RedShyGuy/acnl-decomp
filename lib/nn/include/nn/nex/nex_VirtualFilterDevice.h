#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// The addresses that the packets of the network emulation are filtered by (RootTransport +0x100).
// The names of the members are ours.
class VirtualFilterDevice
{
public:
    // (inline in the constructor of RootTransport)
    VirtualFilterDevice() : m_Unknown0x00(0) {}

    // true if the address (or any address with its port) is one of the addresses
    bool Match(const nn::nex::InetAddress& address); // 0x00394B98 | fefates:bytes [tier B]

    u32 m_Unknown0x00;                   // 0x00
    std::set<InetAddress> m_Addresses;   // 0x04
};
ASSERT_SIZE(VirtualFilterDevice, 0x20);
} // namespace nex
} // namespace nn

namespace __rw {
// (out of line in the original; nex_VirtualFilterDevice.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, nn::nex::InetAddress, __ident<nn::nex::InetAddress, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
                          nn::nex::MemAllocator<nn::nex::InetAddress> >::iterator
__rb_tree<nn::nex::InetAddress, nn::nex::InetAddress, __ident<nn::nex::InetAddress, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<nn::nex::InetAddress> >::erase(iterator first, iterator last);
} // namespace __rw
