#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Network.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::Network::Network()
{
}

// 0x003D3434 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::Network::~Network()
{
}

// 0x003D2980 | fefates:bytes [tier B]
void nn::nex::Network::ReleaseInstance()
{
}

} // namespace nex
} // namespace nn
