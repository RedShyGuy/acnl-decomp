#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_SocketStreamBase.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E8000 slot 0x00 | fefates:bytes
nn::pia::inet::SocketStreamBase::~SocketStreamBase()
{
}

// 0x003E7FE4 slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
void nn::pia::inet::SocketStreamBase::vf_0x04()
{
}

// 0x003E7FAC | fefates:bytes [tier B]
nn::pia::inet::SocketStreamBase::SocketStreamBase()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
