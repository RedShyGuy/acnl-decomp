#include "nn/nex/nex_PRUDPEndPointMap.h"

namespace nn {
namespace nex {
// 0x003833E4 | fefates:bytes [tier B]
nn::nex::PRUDPEndPoint* nn::nex::PRUDPEndPointMap::Locate(const nn::nex::InetAddress& address, nn::nex::VirtualPort port)
{
    iterator it = find(EndPointUniqueID(address, port));
    if (it == end()) {
        return nullptr;
    }
    return it->second;
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0082E288 | mk7dlp:bytes [tier A]
template <>
DECOMP_NOINLINE map<nn::nex::EndPointUniqueID, nn::nex::PRUDPEndPoint*>::iterator map<nn::nex::EndPointUniqueID, nn::nex::PRUDPEndPoint*>::find(const nn::nex::EndPointUniqueID& key)
{
    return m_Tree.find(key);
}
} // namespace std
