#include "nn/socket/detail/detail_Api.h"

namespace nn {
namespace socket {
namespace detail {
// 0x00484E74 | fefates:bytes [tier B]
void SetSockOpt(int*, int, int, int, const unsigned char*, int)
{
}

// 0x00484EE0 | fefates:bytes [tier B]
void GetAddrInfo(int*, const char*, unsigned int, const char*, unsigned int, const unsigned char*, int, int*, unsigned char*, unsigned int)
{
}

// 0x004862F8 | fefates:bytes [tier B]
void GetHostByName(int*, const char*, unsigned int, unsigned char*, unsigned int)
{
}

// 0x00486780 | fefates:bytes [tier B]
void GetNetworkOpt(int*, int, int, unsigned char*, int*)
{
}

// 0x004867E4 | fefates:bytes [tier B]
void RecvFromSmall(int*, int, unsigned char*, int, int, unsigned char*, unsigned int)
{
}

// 0x00486DCC | fefates:bytes [tier B]
void SendToSmallMulti(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int, unsigned int)
{
}

// 0x00487284 | fefates:bytes-fuzzy [tier B]
void ConvertErrorResult(nn::Result)
{
}

// 0x00487390 | fefates:bytes [tier B]
void FinalizeSessionPool()
{
}

// 0x0048746C | fefates:bytes [tier B]
void InitializeDnsClient()
{
}

// 0x00487528 | fefates:bytes [tier B]
void InitializeSessionPool(nn::fnd::IAllocator&, int)
{
}

// 0x00487960 | fefates:bytes [tier B]
void FinalizeControlSession()
{
}

// 0x004879B0 | fefates:bytes [tier B]
void InitializeControlSession(unsigned int, unsigned int)
{
}

// 0x00487A78 | mk7dlp:callgraph [tier A]
void GetRequiredMemorySizeForSessionPool(int)
{
}

// 0x00487F14 | fefates:bytes [tier B]
void Free(void*)
{
}

// 0x00487F48 | fefates:bytes [tier B]
void Poll(int*, nn::socket::PollFd*, unsigned int, int)
{
}

// 0x00488C24 | fefates:bytes [tier B]
void Close(int*, int)
{
}

// 0x00489AB8 | fefates:bytes [tier B]
void Allocate(unsigned int, int)
{
}

// 0x00489FB8 | fefates:bytes [tier B]
void Shutdown(int*, int, int)
{
}

// 0x0048A4EC | fefates:bytes [tier B]
void GetHostId(unsigned int*)
{
}

} // namespace detail
} // namespace socket
} // namespace nn
