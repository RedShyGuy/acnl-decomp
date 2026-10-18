#include "nn/nex/nex_VirtualFilterDevice.h"

namespace nn {
namespace nex {
// 0x00394B98 | fefates:bytes [tier B]
bool nn::nex::VirtualFilterDevice::Match(const nn::nex::InetAddress& address)
{
    if (m_Addresses.find(address) != m_Addresses.end()) {
        return true;
    }
    InetAddress anyAddress;
    anyAddress.m_SockAddr.m_Address = __builtin_bswap32(0);
    anyAddress.m_SockAddr.m_Port = __builtin_bswap16(__builtin_bswap16(address.m_SockAddr.m_Port));
    return m_Addresses.find(anyAddress) != m_Addresses.end();
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x007F70BC | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, nn::nex::InetAddress, __ident<nn::nex::InetAddress, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
                          nn::nex::MemAllocator<nn::nex::InetAddress> >::iterator
__rb_tree<nn::nex::InetAddress, nn::nex::InetAddress, __ident<nn::nex::InetAddress, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<nn::nex::InetAddress> >::erase(iterator first, iterator last)
{
    return EraseRangeAllInline(first, last);
}

// 0x007F7858 | mk7dlp:callgraph [tier A]
template void __rb_tree<nn::nex::InetAddress, nn::nex::InetAddress, __ident<nn::nex::InetAddress, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
                        nn::nex::MemAllocator<nn::nex::InetAddress> >::_C_erase(Node* pNode);
} // namespace __rw
