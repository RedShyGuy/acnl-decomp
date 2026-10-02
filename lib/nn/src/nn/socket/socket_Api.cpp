#include "nn/socket/socket_Api.h"

namespace nn {
namespace socket {
// 0x00484A18 | mk7dlp:callseq [tier A]
void Initialize(unsigned, unsigned, int, int)
{
}

// 0x00484BA8 | fefates:bytes [tier B]
void GetAddrInfo(const char*, const char*, const nn::socket::AddrInfo*, nn::socket::AddrInfo**)
{
}

// 0x00484C24 | fefates:bytes [tier B]
void GetPrimaryAddress(unsigned char*, unsigned char*)
{
}

// 0x00484CA0 | mk7dlp:bytes [tier A]
void GetRequiredMemorySize(unsigned, int)
{
}

// 0x00484CC0 | fefates:bytes [tier B]
void IPAtoN(const char*, unsigned char*)
{
}

// 0x0048A550 | fefates:bytes [tier B]
void Finalize()
{
}

// 0x0048A628 | fefates:bytes [tier B]
void InetNtoP(int, const void*, char*, unsigned int)
{
}

// 0x0048A6A0 | fefates:bytes [tier B]
void InetPtoN(int, const char*, void*)
{
}

} // namespace socket
} // namespace nn
