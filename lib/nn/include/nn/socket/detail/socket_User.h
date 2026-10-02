#pragma once

#include "decomp.h"

namespace nn {
namespace socket {
namespace detail {
class User
{
public:
    void SetSockOpt(int*, int, int, int, const unsigned char*, int); // 0x004883DC | fefates:bytes [tier A]
    void GetAddrInfo(int*, const char*, unsigned int, const char*, unsigned int, const unsigned char*, int, int*, unsigned char*, unsigned int); // 0x00488444 | fefates:bytes [tier A]
    void GetSockName(int*, int, unsigned char*, unsigned int); // 0x0048850C | fefates:bytes [tier A]
    void SendToSmall(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int); // 0x00488578 | fefates:bytes [tier A]
    void AttachProcess(nn::Handle, unsigned int); // 0x004885FC | fefates:callgraph [tier A]
    void GetHostByName(int*, const char*, unsigned int, unsigned char*, unsigned int); // 0x00488668 | fefates:bytes [tier A]
    void GetNetworkOpt(int*, int, int, unsigned char*, int*); // 0x004886E4 | fefates:bytes [tier B]
    void RecvFromSmall(int*, int, unsigned char*, int*, int, unsigned char*, unsigned int); // 0x00488760 | fefates:bytes [tier B]
    void SendToSmallMulti(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int, unsigned int); // 0x00488804 | fefates:bytes [tier A]
    void Bind(int*, int, const unsigned char*, unsigned int); // 0x0048887C | fefates:bytes [tier A]
    void Poll(int*, const nn::socket::PollFd*, nn::socket::PollFd*, unsigned int, int); // 0x004888DC | fefates:bytes [tier B]
    void Close(int*, int); // 0x00488968 | fefates:bytes [tier B]
    void Fcntl(int*, int, int, int); // 0x004889AC | fefates:bytes [tier A]
    void SendTo(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int); // 0x004889F8 | fefates:bytes [tier A]
    void Socket(int*, int, int, int); // 0x00488A7C | fefates:bytes [tier A]
    void RecvFrom(int*, int, unsigned char*, int, int, unsigned char*, unsigned int); // 0x00488B28 | fefates:bytes [tier A]
    void Shutdown(int*, int, int); // 0x00488BAC | fefates:bytes [tier B]
    void GetHostId(unsigned int*); // 0x00488BF0 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace socket
} // namespace nn
